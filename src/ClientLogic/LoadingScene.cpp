//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file LoadingScene.cpp Loading scene
//

// includes
#include "stdafx.h"
#include "LoadingScene.hpp"
#include "GraphicsTaskManager.hpp"
#include "GameClientBase.hpp"
#include "WindowManager.hpp"
#include "ProgressBar.hpp"

LoadingScene::LoadingScene(ISceneManager& sceneManager, GameClientBase& gameClient, IFileSystem& fileSystem)
:Scene(sceneManager),
 m_gameClient(gameClient),
 m_preloadManager(gameClient.GetGraphicsTaskManager())
{
   SetupUI(fileSystem);
}

void LoadingScene::FinishQueue(T_fnFinishedLoading fnFinishedLoading)
{
   m_preloadManager.FinishQueue(fnFinishedLoading);
}

void LoadingScene::SetupUI(IFileSystem& fileSystem)
{
   IWindowManager& windowManager = m_gameClient.GetWindowManager();

   WindowManager::CreateFromJson(windowManager.GetRootWindow(), fileSystem, _T("ui/LoadingScreen.json"));
}

void LoadingScene::PreRender()
{
   Scene::PreRender();

   IWindowManager& windowManager = m_gameClient.GetWindowManager();

   std::shared_ptr<ProgressBar> spProgress = windowManager.FindWindow<ProgressBar>(_T("progress"));
   if (spProgress != nullptr)
      spProgress->Current(m_preloadManager.PercentDone());
}
