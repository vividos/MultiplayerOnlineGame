//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Scenegraph.cpp Arena scenegraph
//

// includes
#include "StdAfx.h"
#include "Scenegraph.hpp"
#include "Checkerboard.hpp"
#include "ViewModel.hpp"
#include "ObjectMap.hpp"
#include "Mobile.hpp"
#include "Player.hpp"
#include "GraphicsTaskManager.hpp"
#include "RenderEngine.hpp"
#include "ModelRenderManager.hpp"
#include "ModelRenderInstance.hpp"

using Arena::Scenegraph;

Scenegraph::Scenegraph(Arena::ViewModel& viewModel,
   IFileSystem& fileSystem,
   GraphicsTaskManager& taskManager,
   RenderEngine& renderEngine)
:m_viewModel(viewModel),
 m_renderEngine(renderEngine),
 m_renderContainer(taskManager),
 m_spSkyRenderManager(new SkyRenderManager(taskManager, fileSystem)),
 m_spModelRenderManager(new ModelRenderManager(fileSystem, taskManager))
{
   m_renderContainer.Add(m_spSkyRenderManager);

   m_renderContainer.Add(std::shared_ptr<IRenderable>(new Checkerboard(0, 256, 0, 256, -1.0)));

   m_renderContainer.Add(m_spModelRenderManager);

   InitPlayer();
   InitMobiles();

   // register events
   m_viewModel.AddObjectEvent().Add(std::bind(&Scenegraph::AddMobile, this, std::placeholders::_1));
   m_viewModel.RemoveObjectEvent().Add(std::bind(&Scenegraph::RemoveMobile, this, std::placeholders::_1));
   m_viewModel.UpdateObjectEvent().Add(std::bind(&Scenegraph::UpdateMobile, this, std::placeholders::_1, std::placeholders::_2));
   m_viewModel.UpdatePlayerEvent().Add(std::bind(&Scenegraph::UpdatePlayer, this, std::placeholders::_1));
}

void Scenegraph::AddMobile(MobilePtr spMobile)
{
   ATLASSERT(spMobile != NULL);
   ATLASSERT(m_spModelRenderManager != NULL);

   m_spModelRenderManager->AddMobile(spMobile);
}

void Scenegraph::RemoveMobile(const ObjectId& id)
{
   ATLASSERT(id != ObjectId::Null());

   m_spModelRenderManager->RemoveMobile(id);
}

void Scenegraph::UpdateMobile(const ObjectId& id, const MovementInfo& info)
{
   ATLASSERT(id != ObjectId::Null());

   m_spModelRenderManager->UpdateMobile(id, info);
}

void Scenegraph::UpdatePlayer(const Player& player)
{
   m_spModelRenderManager->UpdatePlayer(player);
}

void Scenegraph::InitMobiles()
{
   const ObjectMap& objMap = m_viewModel.GetObjectMap();
   ObjectMap::T_mapObjects::const_iterator iter = objMap.GetMap().begin();
   ObjectMap::T_mapObjects::const_iterator stop = objMap.GetMap().end();

   for (; iter != stop; ++iter)
      AddMobile(std::dynamic_pointer_cast<Mobile>(iter->second));
}

void Scenegraph::InitPlayer()
{
   std::shared_ptr<Player> spPlayer = m_viewModel.GetPlayer();

   m_spModelRenderManager->InitPlayer(*spPlayer);
}

void Scenegraph::SetPosition(const Vector3d& /*vPosition*/, double /*dAngleDir*/)
{
   // TODO terrain position for LOD
}

void Scenegraph::UpdateSkyRenderManager()
{
#ifdef _DEBUG
   static DateTime s_dtStart = DateTime::Now();
   DateTime dtNow = s_dtStart + TimeSpan(0, int(GetTickCount() / 1000.0 * 20.0), 0);
#else
   // use real time for release
   DateTime dtNow = DateTime::Now();
#endif

   m_spSkyRenderManager->SetCurrentDateTime(dtNow);
}

void Scenegraph::Render(RenderOptions& options)
{
   // update player transparency before rendering
   //m_spModelRenderManager->GetPlayerRenderInstance().SetTransparency(m_viewModel.GetPlayerTransparency());

   const Vector3d& vPos = m_viewModel.GetPlayer()->Pos();
   glTranslated(-vPos.X(), -vPos.Y(), -vPos.Z());

   UpdateSkyRenderManager();

   m_renderContainer.Render(options);
}
