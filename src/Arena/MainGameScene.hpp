//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ArenaMainGameScene.hpp Arena main game scene
//
#pragma once

// includes
#include "Scene.hpp"
#include "ViewModel.hpp"
#include "MusicDirector.hpp"

namespace Arena
{
// forward references
class GameClient;
class Game;

/// arena main game scene
class MainGameScene: public Scene
{
   typedef Scene BaseClass;

public:
   /// ctor
   MainGameScene(ISceneManager& sceneManager, GameClient& gameClient, Game& game) throw();
   /// dtor
   virtual ~MainGameScene() throw() {}

private:
   /// view model
   Arena::ViewModel m_viewModel;

   /// music director
   Audio::MusicDirector m_musicDirector;
};

} // namespace Arena
