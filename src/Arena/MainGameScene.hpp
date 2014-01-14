//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MainGameScene.hpp Arena main game scene
//
#pragma once

// includes
#include "Scene.hpp"
#include "ViewModel.hpp"
#include "MusicDirector.hpp"

// forward references
class PreloadManager;

namespace Arena
{
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

   /// prepares some assets
   void Prepare(PreloadManager& preloadManager);

private:
   /// view model
   Arena::ViewModel m_viewModel;

   /// music director
   Audio::MusicDirector m_musicDirector;
};

} // namespace Arena
