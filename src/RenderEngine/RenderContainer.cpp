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

void RenderContainer::Add(std::shared_ptr<IRenderable> spRenderObject, unsigned int uiPriority)
{
   QueueItem item(spRenderObject, uiPriority);

   if (spRenderObject->IsPrepareNeeded())
   {
      m_taskManager.BackgroundTaskGroup().Add(
         std::bind(&RenderContainer::AsyncPrepare, this, item));
   }
   else
   if (spRenderObject->IsUploadNeeded())
   {
      // no prepare, but update
      m_taskManager.UploadTaskGroup().Add(
         std::bind(&RenderContainer::AsyncUpload, this, item));
   }
   else
   {
      // no prepare, no update: just add
      m_taskManager.UploadTaskGroup().Add(
         std::bind(&RenderContainer::InternalAdd, this, item));
   }
}

void RenderContainer::Remove(std::shared_ptr<IRenderable> spRenderObject)
{
   // no prepare, no update: just add
   m_taskManager.UploadTaskGroup().Add(
      std::bind(&RenderContainer::InternalRemove, this, spRenderObject));
}

void RenderContainer::Render(RenderOptions& options)
{
   std::for_each(m_setRenderObjects.begin(), m_setRenderObjects.end(),
      [&options](const QueueItem& item){
         item.m_spRenderable->Render(options);
   });
}

void RenderContainer::AsyncPrepare(QueueItem item)
{
   ATLASSERT(item.m_spRenderable->IsPrepareNeeded());

   item.m_spRenderable->Prepare();

   if (item.m_spRenderable->IsUploadNeeded())
   {
      m_taskManager.UploadTaskGroup().Add(
         std::bind(&RenderContainer::AsyncUpload, this, item));
   }
   else
   {
      // no update: just add (in upload task)
      m_taskManager.UploadTaskGroup().Add(
         std::bind(&RenderContainer::InternalAdd, this, item));
   }
}

void RenderContainer::AsyncUpload(QueueItem item)
{
   ATLASSERT(item.m_spRenderable->IsPrepareNeeded());

   item.m_spRenderable->Upload();

   // since we're already in the upload task, add object right away
   InternalAdd(item);
}

void RenderContainer::InternalAdd(QueueItem item)
{
   m_setRenderObjects.insert(item);
}

void RenderContainer::InternalRemove(std::shared_ptr<IRenderable> spRenderObject)
{
   spRenderObject->Done();

   auto iter = m_setRenderObjects.begin(), stop = m_setRenderObjects.end();
   for (; iter != stop; iter++)
   {
      const QueueItem& item = *iter;

      if (item.m_spRenderable == spRenderObject)
      {
         m_setRenderObjects.erase(iter);
         return;
      }
   }

   ATLASSERT(false); // object must be in container!
}
