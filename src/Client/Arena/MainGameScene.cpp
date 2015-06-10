//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MainGameScene.cpp Main game scene
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
#include "PreloadManager.hpp"
#include "GameMenu/MainPanel.hpp"

using namespace Arena;

MainGameScene::MainGameScene(ISceneManager& sceneManager, GameClient& gameClient, Game& game) throw()
:Scene(sceneManager),
 m_viewModel(game.GetModel()),
 m_musicDirector(Audio::IAudioManager::Get(), game.GetFileSystem()),
 m_gameClient(gameClient),
 m_game(game)
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

   // key bindings
   DefaultActionKeyDefs::Register(
      gameClient.GetKeyboardActionManager());

   gameClient.GetKeyboardActionBindings().RegisterActionHandler(
      KeyboardActionManager::actionGameMenu,
      true, // at key down
      std::bind(&MainGameScene::OnActionKeyGameMenu, this));

   //RenderOptions& options = gameClient.GetRenderEngine().GetRenderOptions();
   //options.Set(RenderOptions::optionSkyMeshLines, true);
}

void MainGameScene::Prepare(PreloadManager& preloadManager)
{
   std::shared_ptr<View> spView = std::dynamic_pointer_cast<View>(m_spView);
   ATLASSERT(spView != nullptr);

   spView->Prepare(preloadManager);
}

void MainGameScene::ActivateView()
{
   std::shared_ptr<View> spView = std::dynamic_pointer_cast<View>(m_spView);
   ATLASSERT(spView != nullptr);

   spView->Activate(m_gameClient.GetRenderEngine());
}

void MainGameScene::OnActionKeyGameMenu()
{
   if (m_spGameMenu == nullptr)
   {
      m_spGameMenu.reset(new GameMenu::MainPanel(m_gameClient.GetWindowManager(), m_game.GetFileSystem()));

      m_spGameMenu->RegisterExitClickedHandler(
         std::bind(&MainGameScene::OnGameMenuExit, this));
   }

   m_spGameMenu->Show(m_gameClient.GetWindowManager());
}

void MainGameScene::OnGameMenuExit()
{
   // exit by setting null scene
   ISceneManager& sceneManager = m_gameClient;
   sceneManager.ChangeScene(nullptr);
}
