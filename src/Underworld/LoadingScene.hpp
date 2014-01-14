//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/LoadingScene.hpp Loading scene
//
#pragma once

// includes
#include "Scene.hpp"

// forward references
class GameClientBase;
class RenderContainer;
struct Game;

/// ingame scene
class LoadingScene: public Scene
{
public:
   /// ctor
   LoadingScene(ISceneManager& sceneManager, GameClientBase& gameClient, Game& game);

private:
   void AsyncLoadGame();

   void ChangeScene();

private:
   /// game client
   GameClientBase& m_gameClient;

   /// game
   Game& m_game;
};
