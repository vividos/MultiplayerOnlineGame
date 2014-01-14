//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file View.cpp Arena view
//

// includes
#include "StdAfx.h"
#include "View.hpp"
#include "RenderEngine.hpp"
#include "ViewModel.hpp"
#include "Player.hpp"
#include "PerspectiveCamera.hpp"
#include "Scenegraph.hpp"

using Arena::View;
using Arena::ViewModel;

View::View(KeyboardActionManager& keyboardActionManager,
   IFileSystem& fileSystem,
   GraphicsTaskManager& taskManager,
   RenderEngine& engine,
   ViewModel& viewModel)
:m_spScenegraph(new Arena::Scenegraph(viewModel, fileSystem, taskManager, engine)),
 m_viewModel(viewModel),
 m_spCamera(new PerspectiveCamera),
 m_actionBindings(keyboardActionManager)
{
   // register action handler
   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionAction1, true,
      std::bind(&ViewModel::DoAction, std::ref(m_viewModel), 1));

   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionSelectNext, true,
      std::bind(&ViewModel::SelectNextMobile, std::ref(m_viewModel), true));

   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionSelectPrev, true,
      std::bind(&ViewModel::SelectNextMobile, std::ref(m_viewModel), false));

   // set scenegraph
   engine.SetScenegraph(m_spScenegraph);

   // init camera
   m_spCamera->SetNearFarPlaneDistance(0.1, 512.0);
   engine.SetCamera(m_spCamera);

   m_spCamera->SetPosition(Vector3d(6.95, 1.86, 6.27), 134.6, -14.5);
   //m_spCamera->SetPosition(Vector3d(-0.766, 3.066, -4.491), 145.1, -19.1);
}

void View::PreRender()
{
   ATLASSERT(m_spScenegraph != nullptr);

   m_spScenegraph->SetPosition(m_viewModel.GetPlayer()->Pos(), m_viewModel.GetPlayer()->ViewAngle());
}
