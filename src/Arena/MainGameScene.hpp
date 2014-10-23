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

namespace GameMenu
{
   class MainPanel;
}

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
   /// called when the game menu action key is pressed
   void OnActionKeyGameMenu();

   /// called when "exit" button is pressed on game menu
   void OnGameMenuExit();

private:
   /// view model
   Arena::ViewModel m_viewModel;

   /// music director
   Audio::MusicDirector m_musicDirector;

   /// ref to game client
   GameClient& m_gameClient;

   /// ref to game object
   Game& m_game;

   /// game menu
   std::shared_ptr<GameMenu::MainPanel> m_spGameMenu;
};

} // namespace Arena
