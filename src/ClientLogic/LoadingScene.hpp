//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file LoadingScene.hpp Loading scene
//
#pragma once

// includes
#include "ClientLogic.hpp"
#include "Scene.hpp"
#include "PreloadManager.hpp"
#include <atomic>

// forward references
//class GraphicsTaskManager;
class GameClientBase;
class RenderContainer;
class IWindowManager;
class IFileSystem;

/// \brief Loading scene
/// Shows a progress bar while pre-loading assets for the game. Use as follows:
/// 1. Create LoadingScene object and call ISceneManager::ChangeScene().
/// 2. Use PreloadManager to add background tasks using AddBackgroundTask().
/// 3. Call FinishQueue() and add a handler to change to your next scene.
class CLIENTLOGIC_DECLSPEC LoadingScene: public Scene
{
public:
   /// function type to call when loading has finished (e.g. to change scene)
   typedef std::function<void()> T_fnFinishedLoading;

   /// ctor
   LoadingScene(ISceneManager& sceneManager, GameClientBase& gameClient, IFileSystem& fileSystem);

   /// returns preload manager
   PreloadManager& GetPreloadManager() { return m_preloadManager; }

   /// finishes background queue for preload manager
   void FinishQueue(T_fnFinishedLoading fnFinishedLoading);

private:
   /// sets up UI
   void SetupUI(IFileSystem& fileSystem);

   /// async finish queue handler
   void AsyncFinishQueue(T_fnFinishedLoading fnFinishedLoading);

   // virtual methods from Scene

   /// is called before rendering a scene
   virtual void PreRender() override;

private:
   /// game client
   GameClientBase& m_gameClient;

   /// preload manager
   PreloadManager m_preloadManager;
};
