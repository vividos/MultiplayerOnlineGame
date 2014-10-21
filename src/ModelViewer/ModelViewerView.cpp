//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ModelViewerView.cpp Model viewer view
//

// includes
#include "StdAfx.h"
#include "ModelViewerView.hpp"
#include "ModelViewerViewModel.hpp"
#include "RenderEngine.hpp"
#include "PerspectiveCamera.hpp"
#include "Checkerboard.hpp"
#include "Player.hpp"
#include "ModelRenderInstance.hpp"

using ModelViewer::View;
using ModelViewer::ViewModel;

View::View(ViewModel& viewModel,
   IFileSystem& fileSystem,
   RenderEngine& renderEngine,
   GraphicsTaskManager& taskManager)
:m_viewModel(viewModel),
 m_engine(renderEngine),
 m_modelRenderManager(fileSystem, taskManager),
 m_bModelInited(false)
{
   // init camera
   m_spCamera.reset(new PerspectiveCamera);
   m_engine.SetCamera(m_spCamera);

   // init lighting
   OpenGL::LightSource ls(Color::White(), Vector3d(4.0, 4.0, -4.0));
   m_lightingManager.Add(ls);
   m_lightingManager.ColorTracking(true);

   // subscribe to events
   m_viewModel.ModelDescriptionChangedEvent().Add(
      std::bind(&View::OnModelDescriptionChanged, this, std::placeholders::_1));

   m_viewModel.AnimationChangedEvent().Add(
      std::bind(&View::OnAnimationChanged, this, std::placeholders::_1));
}

void View::OnModelDescriptionChanged(Player& player)
{
   if (!m_bModelInited)
   {
      m_bModelInited = true;
      m_modelRenderManager.InitPlayer(player);
   }
   else
      m_modelRenderManager.UpdatePlayer(player);
}

void View::OnAnimationChanged(unsigned int uiCurrentAnimation)
{
   CString cszAnimationName = m_viewModel.AnimationName(uiCurrentAnimation);

   // re-init state for model
   unsigned int uiStartFrame = 0, uiEndFrame = 0;
   m_viewModel.GetAnimationFrames(cszAnimationName, uiStartFrame, uiEndFrame);

   ModelRenderInstance& inst = m_modelRenderManager.GetPlayerRenderInstance();
   inst.DisplayState()->SetAnimationFrames(uiStartFrame, uiEndFrame, true, true);
}

void View::Render(RenderOptions& options)
{
   Checkerboard c(-8, 8, -8, 8);
   c.Render(options);

   m_lightingManager.Enable();

   m_modelRenderManager.Render(options);

   m_lightingManager.Disable();
}
