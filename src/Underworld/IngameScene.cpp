//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/IngameScene.cpp Ingame scene
//

// includes
#include "stdafx.h"
#include "IngameScene.hpp"
#include "DefaultActionKeyDefs.hpp"
#include "RenderEngine.hpp"
#include "PerspectiveCamera.hpp"
#include "OverviewController.hpp"
#include "Renderer.hpp"
#include "Game.hpp"

IngameScene::IngameScene(ISceneManager& sceneManager, RenderEngine& renderEngine, KeyboardActionManager& keyboardActionManager, Game& game)
:Scene(sceneManager),
 m_game(game),
 m_renderEngine(renderEngine)
{
   DefaultActionKeyDefs::Register(keyboardActionManager);

   InitRenderer();

   m_spController.reset(new OverviewController(keyboardActionManager, m_spCamera));
}

void IngameScene::InitRenderer()
{
   m_spRenderer.reset(new Renderer);

   m_renderEngine.SetScenegraph(m_spRenderer);

   m_spRenderer->Load(m_game.m_fileSystem);

   m_spRenderer->LoadNewLevel(m_game.m_world.GetLevelList().GetLevel(0));

   m_spRenderer->Upload();

   m_spCamera.reset(new PerspectiveCamera);

   m_spCamera->SetPosition(Vector3d(-32.0, 12.0, 0.0), -184.0, -44.0);
   m_spCamera->SetNearFarPlaneDistance(0.1, 100.0); // max distance would be about 90.0, so we should be safe here

   m_renderEngine.SetCamera(m_spCamera);
}
