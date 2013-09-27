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

/// all game objects
struct Game
{
   /// underworld
   Underworld::World m_world;

   /// file system
   VirtualFileSystem m_fileSystem;

   /// game strings
   GameStrings m_strings;

   /// conversation manager
   std::unique_ptr<ConvManager> m_upConvManager;

   /// inits game objects
   void Init();

   /// cleans up game objects
   void Done();
};
