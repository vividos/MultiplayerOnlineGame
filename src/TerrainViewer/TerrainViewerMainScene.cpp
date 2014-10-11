//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TerrainViewerMainScene.cpp Terrain viewer scene
//

// includes
#include "StdAfx.h"
#include "TerrainViewerMainScene.hpp"
#include "RenderEngine.hpp"
#include "RenderOptions.hpp"
#include "DefaultActionKeyDefs.hpp"
#include "OverviewController.hpp"
#include <functional>

TerrainViewer::MainScene::MainScene(
   ISceneManager& sceneManager,
   GraphicsTaskManager& graphicsTaskManager,
   RenderEngine& renderEngine,
   KeyboardActionManager& keyboardActionManager)
:Scene(sceneManager),
m_keyboardActionBindings(keyboardActionManager)
{
   std::shared_ptr<View> spView(
      new View(renderEngine, graphicsTaskManager, m_fileSystem));
   m_spView = spView;

   // register some keys
   DefaultActionKeyDefs::Register(keyboardActionManager);

   // actions
   m_keyboardActionBindings.RegisterActionHandler(KeyboardActionManager::actionAction1, false,
      std::bind(&TerrainViewer::MainScene::ToggleFillMode, this));

   //m_keyboardActionBindings.RegisterActionHandler(KeyboardActionManager::actionAction2, false,
   //   std::bind(&TerrainViewer::MainScene::ToggleRenderMode, this));

   // set scenegraph
   renderEngine.SetScenegraph(spView);

   // set controller
   std::shared_ptr<OverviewController> spController(
      new OverviewController(keyboardActionManager, spView->GetCamera()));

   spController->SetPosition(Vector3d(188.0, 267.0, 480.0), -347.0, -64.0);
   spController->SetMovementSpeed(200.0);

   m_spController = spController;

   // set render options
   renderEngine.GetRenderOptions().Set(RenderOptions::optionTerrainFrustumCulling, false);
   renderEngine.GetRenderOptions().Set(RenderOptions::optionTerrainModelNormals, false);
   renderEngine.GetRenderOptions().Set(RenderOptions::optionTerrainZoneBoundingBox, false);
}

void TerrainViewer::MainScene::ToggleFillMode()
{
   std::shared_ptr<View> spView = std::dynamic_pointer_cast<View>(m_spView);

   ATLASSERT(spView != nullptr);

   spView->FillMode(!spView->FillMode());
}
