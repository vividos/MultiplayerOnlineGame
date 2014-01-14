//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file RenderContainer.cpp Render container
//

// includes
#include "StdAfx.h"
#include "RenderContainer.hpp"
#include "IRenderable.hpp"
#include "GraphicsTaskManager.hpp"
#include <boost/foreach.hpp>

RenderContainer::RenderContainer(GraphicsTaskManager& taskManager)
:m_taskManager(taskManager)
{
}

void RenderContainer::Add(std::shared_ptr<IRenderable> spRenderObject)
{
   if (spRenderObject->IsPrepareNeeded())
   {
      m_taskManager.BackgroundTaskGroup().Add(
         boost::bind(&RenderContainer::AsyncPrepare, this, spRenderObject));
   }
   else
   if (spRenderObject->IsUploadNeeded())
   {
      // no prepare, but update
      m_taskManager.UploadTaskGroup().Add(
         boost::bind(&RenderContainer::AsyncUpload, this, spRenderObject));
   }
   else
   {
      // no prepare, no update: just add
      m_taskManager.UploadTaskGroup().Add(
         boost::bind(&RenderContainer::InternalAdd, this, spRenderObject));
   }
}

void RenderContainer::Remove(std::shared_ptr<IRenderable> spRenderObject)
{
   // no prepare, no update: just add
   m_taskManager.UploadTaskGroup().Add(
      boost::bind(&RenderContainer::InternalRemove, this, spRenderObject));
}

void RenderContainer::Render(RenderOptions& options)
{
   BOOST_FOREACH(std::shared_ptr<IRenderable> spRenderObject, m_vecRenderObjects)
      spRenderObject->Render(options);
}

void RenderContainer::AsyncPrepare(std::shared_ptr<IRenderable> spRenderObject)
{
   ATLASSERT(spRenderObject->IsPrepareNeeded());

   spRenderObject->Prepare();

   if (spRenderObject->IsUploadNeeded())
   {
      m_taskManager.UploadTaskGroup().Add(
         boost::bind(&RenderContainer::AsyncUpload, this, spRenderObject));
   }
   else
   {
      // no update: just add (in upload task)
      m_taskManager.UploadTaskGroup().Add(
         boost::bind(&RenderContainer::InternalAdd, this, spRenderObject));
   }
}

void RenderContainer::AsyncUpload(std::shared_ptr<IRenderable> spRenderObject)
{
   ATLASSERT(spRenderObject->IsPrepareNeeded());

   spRenderObject->Upload();

   // since we're already in the upload task, add object right away
   InternalAdd(spRenderObject);
}

void RenderContainer::InternalAdd(std::shared_ptr<IRenderable> spRenderObject)
{
   m_vecRenderObjects.push_back(spRenderObject);
}

void RenderContainer::InternalRemove(std::shared_ptr<IRenderable> spRenderObject)
{
   spRenderObject->Done();

   for (size_t i=0, iMax=m_vecRenderObjects.size(); i<iMax; i++)
   {
      if (spRenderObject == m_vecRenderObjects[i])
      {
         m_vecRenderObjects.erase(m_vecRenderObjects.begin() + i);
         return;
      }
   }

   ATLASSERT(false); // object should be in container!
}
