//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TerrainViewerMainScene.hpp Terrain viewer scene
//
#pragma once

// includes
#include "TerrainViewerView.hpp"
#include "Scene.hpp"
#include "GraphicsTaskManager.hpp"
#include "KeyboardActionBindings.hpp"
#include "VirtualFileSystem.hpp"

// forward references
class RenderEngine;
class KeyboardActionManager;

namespace TerrainViewer
{

/// terrain viewer scene
class MainScene : public Scene
{
public:
   /// ctor
   MainScene(
      ISceneManager& sceneManager,
      GraphicsTaskManager& graphicsTaskManager,
      RenderEngine& renderEngine,
      KeyboardActionManager& keyboardActionManager);
   /// dtor
   virtual ~MainScene(){}

private:
   /// toggles fill mode
   void ToggleFillMode();

private:
   /// keyboard bindings
   KeyboardActionBindings m_keyboardActionBindings;

   /// file system
   VirtualFileSystem m_fileSystem;
};

} // namespace TerrainViewer
