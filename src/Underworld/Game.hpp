//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/Game.hpp Game object
//
#pragma once

// includes
#include "stdafx.h"
#include "Underworld.hpp"
#include "VirtualFileSystem.hpp"
#include "GameStrings.hpp"
#include "ConvManager.hpp"

#include "RenderContainer.hpp"
#include "Renderer.hpp"

// forward references
class GraphicsTaskManager;

/// all game objects
struct Game
{
   Game(GraphicsTaskManager& graphicsTaskManager)
      :m_renderContainer(graphicsTaskManager)
   {
   }

   // model

   /// underworld
   Underworld::World m_world;

   /// file system
   VirtualFileSystem m_fileSystem;

   /// game strings
   GameStrings m_strings;

   /// conversation manager
   std::unique_ptr<ConvManager> m_upConvManager;

   // view

   /// render container
   RenderContainer m_renderContainer;

   /// renderer
   std::shared_ptr<Renderer> m_spRenderer;

   /// inits game objects
   void Init();

   /// cleans up game objects
   void Done();
};
