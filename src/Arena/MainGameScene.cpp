//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file MainGameScene.cpp Main game scene
//

// includes
#include "StdAfx.h"
#include "MainGameScene.hpp"
#include "RenderEngine.hpp"
#include "KeyboardActionManager.hpp"
#include "GraphicsTaskManager.hpp"
#include "IAudioManager.hpp"
#include "DefaultActionKeyDefs.hpp"
#include "IModel.hpp"
#include "GameClient.hpp"
#include "Game.hpp"
#include "ThirdPersonViewController.hpp"
#include "OverviewController.hpp"
#include "PerspectiveCamera.hpp"
#include "View.hpp"
#include "RenderOptions.hpp"

using namespace Arena;

MainGameScene::MainGameScene(ISceneManager& sceneManager, GameClient& gameClient, Game& game) throw()
:Scene(sceneManager),
 m_viewModel(game.GetModel()),
 m_musicDirector(Audio::IAudioManager::Get(), game.GetFileSystem())
{
   std::shared_ptr<View> spView(new View(
      gameClient.GetKeyboardActionManager(),
      game.GetFileSystem(),
      gameClient.GetGraphicsTaskManager(),
      gameClient.GetRenderEngine(),
      m_viewModel));

   m_spView = spView;

#if 0
   ThirdPersonViewController::T_fnSetCameraPosition fnSetCameraPosition =
      std::bind(
         &PerspectiveCamera::SetPosition, spView->GetCamera(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

   m_spController.reset(new ThirdPersonViewController(gameClient.GetKeyboardActionManager(), sceneManager, m_viewModel, fnSetCameraPosition));
#else
   m_spController.reset(new OverviewController(gameClient.GetKeyboardActionManager(), spView->GetCamera()));
#endif

   DefaultActionKeyDefs::Register(
      gameClient.GetKeyboardActionManager());

   RenderOptions& options = gameClient.GetRenderEngine().GetRenderOptions();
   //options.Set(RenderOptions::optionModelFilled, true);
   options.Set(RenderOptions::optionSkyMeshLines, true);
}
