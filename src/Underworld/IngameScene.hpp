//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/IngameScene.hpp Ingame scene
//
#pragma once

// includes
#include "Scene.hpp"

// forward references
struct Game;
class ISceneManager;
class RenderEngine;
class KeyboardActionManager;
class Renderer;
class PerspectiveCamera;

/// ingame scene
class IngameScene: public Scene
{
public:
   /// ctor
   IngameScene(ISceneManager& sceneManager, RenderEngine& renderEngine, KeyboardActionManager& keyboardActionManager, Game& game);

private:
   /// inits renderer
   void InitRenderer();

private:
   /// game
   Game& m_game;

   /// render engine
   RenderEngine& m_renderEngine;

   /// game renderer
   std::shared_ptr<Renderer> m_spRenderer;

   /// camera
   std::shared_ptr<PerspectiveCamera> m_spCamera;
};
