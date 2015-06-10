//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/LoadingScene.cpp Loading scene
//

// includes
#include "stdafx.h"
#include "LoadingScene.hpp"
#include "Renderer.hpp"
#include "Game.hpp"
#include "GraphicsTaskManager.hpp"
#include "GameClientBase.hpp"
#include "IngameScene.hpp"

LoadingScene::LoadingScene(ISceneManager& sceneManager, GameClientBase& gameClient, Game& game)
:Scene(sceneManager),
 m_gameClient(gameClient),
 m_game(game)
{
   gameClient.GetGraphicsTaskManager().BackgroundTaskGroup().Add(std::bind(&LoadingScene::AsyncLoadGame, this));
}

void LoadingScene::AsyncLoadGame()
{
   m_game.Init();

   std::shared_ptr<Renderer> spRenderer = std::make_shared<Renderer>(m_game.m_fileSystem);

   m_game.m_spRenderer = spRenderer;

   m_game.m_renderContainer.Add(spRenderer);

   spRenderer->AsyncLoadNewLevel(m_gameClient.GetGraphicsTaskManager(), m_game.m_world.GetLevelList().GetLevel(0));

   m_sceneManager.DispatchInEventLoop(std::bind(&LoadingScene::ChangeScene, this));
}

void LoadingScene::ChangeScene()
{
   std::shared_ptr<Scene> spScene =
      std::make_shared<IngameScene>(m_sceneManager, m_gameClient.GetRenderEngine(), m_gameClient.GetKeyboardActionManager(), m_game);
   m_sceneManager.ChangeScene(spScene);
}
