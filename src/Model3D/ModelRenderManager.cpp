//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ModelRenderManager.cpp Model render manager
//

// includes
#include "StdAfx.h"
#include "ModelRenderManager.hpp"
#include "ModelRenderInstance.hpp"
#include "Player.hpp"
#include "ObjectMap.hpp"

ModelRenderManager::ModelRenderManager(IFileSystem& fileSystem, GraphicsTaskManager& taskManager)
:m_modelManager(fileSystem, taskManager),
 m_selectedObjectId(ObjectId::Null())
{
}

void ModelRenderManager::InitPlayer(const Player& player)
{
   // first init
   std::shared_ptr<IModelDisplayState> spDisplayState = m_modelManager.Create(player);

   std::shared_ptr<ModelRenderInstance> spPlayerModelRenderInstance(
      new ModelRenderInstance(spDisplayState));

   spPlayerModelRenderInstance->StartFadein();
   spPlayerModelRenderInstance->SetPosition(player.Pos(), player.MovementAngle());

   m_spPlayerModelRenderInstance = spPlayerModelRenderInstance;
}

void ModelRenderManager::UpdatePlayer(const Player& player)
{
   ATLASSERT(m_spPlayerModelRenderInstance != NULL);

   if (m_spPlayerModelRenderInstance == nullptr)
      return;

   m_modelManager.Update(player,
      m_spPlayerModelRenderInstance->DisplayState());

   m_spPlayerModelRenderInstance->SetPosition(player.Pos(), player.ViewAngle());
   m_spPlayerModelRenderInstance->UpdateMovementInfo(player.GetMovementInfo());

   m_selectedObjectId = player.Selection();
}

ModelRenderInstance& ModelRenderManager::GetPlayerRenderInstance()
{
   ATLASSERT(m_spPlayerModelRenderInstance != NULL);

   return *m_spPlayerModelRenderInstance;
}

void ModelRenderManager::AddMobile(MobilePtr spMobile)
{
   const ObjectId& id = spMobile->Id();

   TMapMobileObjectRenderInstances::iterator iter =
      m_mapMobileObjectRenderInstances.find(id);

   if (iter != m_mapMobileObjectRenderInstances.end())
      return; // already in map

   std::shared_ptr<IModelDisplayState> spDisplayState = m_modelManager.Create(*spMobile);

   std::shared_ptr<ModelRenderInstance> spModelRenderInstance(
      new ModelRenderInstance(spDisplayState));

   spModelRenderInstance->StartFadein();
   spModelRenderInstance->SetPosition(spMobile->Pos(), spMobile->MovementAngle());

   m_mapMobileObjectRenderInstances.insert(
      std::make_pair(id, spModelRenderInstance));
}

void ModelRenderManager::RemoveMobile(const ObjectId& id)
{
   ATLASSERT(m_mapMobileObjectRenderInstances.find(id) != m_mapMobileObjectRenderInstances.end());

   // TODO move model to "fadeout" list instead of erasing
   m_mapMobileObjectRenderInstances.erase(id);
   //spPlayerModelRenderInstance->StartFadeout();
}

void ModelRenderManager::UpdateMobile(const ObjectId& id, const MovementInfo& info)
{
   TMapMobileObjectRenderInstances::iterator iter =
      m_mapMobileObjectRenderInstances.find(id);

   if (iter == m_mapMobileObjectRenderInstances.end())
      return;

   iter->second->UpdateMovementInfo(info);
}

void ModelRenderManager::UpdateMobilesToRender(const ObjectMap& objectMap)
{
   auto iter = m_mapMobileObjectRenderInstances.begin();
   auto stop = m_mapMobileObjectRenderInstances.end();

   for (; iter != stop; ++iter)
   {
      ObjectRef objRef = objectMap.FindObject(iter->first);
      ATLASSERT(objRef.m_sp != NULL);

      MobilePtr spMobile = std::dynamic_pointer_cast<Mobile>(objRef.m_sp);
      ATLASSERT(spMobile != NULL);

      std::shared_ptr<ModelRenderInstance> spRenderInstance = iter->second;

      spRenderInstance->SetPosition(spMobile->Pos(), spMobile->ViewAngle());
      spRenderInstance->UpdateMovementInfo(spMobile->GetMovementInfo());

      if (m_selectedObjectId != ObjectId::Null())
      {
         bool bSelected = spMobile->Id() == m_selectedObjectId;
         spRenderInstance->SetSelected(bSelected);
      }
   }
}

void ModelRenderManager::Render(RenderOptions& options)
{
   // all render model functions assume this to be on and don't set it itself
   // to minimize state changes
   glEnable(GL_TEXTURE_2D);

   RenderPlayer(options);
   RenderMobiles(options);
}

void ModelRenderManager::RenderPlayer(RenderOptions& renderOptions)
{
   if (m_spPlayerModelRenderInstance == NULL)
      return;

   m_spPlayerModelRenderInstance->Render(renderOptions);
}

void ModelRenderManager::RenderMobiles(RenderOptions& renderOptions)
{
   auto iter = m_mapMobileObjectRenderInstances.begin();
   auto stop = m_mapMobileObjectRenderInstances.end();

   for (; iter != stop; ++iter)
      iter->second->Render(renderOptions);
}

void ModelRenderManager::Done()
{
   m_spPlayerModelRenderInstance.reset();
   m_mapMobileObjectRenderInstances.clear();
}
