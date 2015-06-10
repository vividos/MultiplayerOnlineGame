//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Arena\Game.hpp Game objects
//
#pragma once

// includes
#include "LocalModel.hpp"
#include "Timer.hpp"
#include "TimeBase.hpp"
#include "VirtualFileSystem.hpp"

/// Arena is a test game that integrates all components in the project
namespace Arena
{

/// Arena game object
class Game
{
public:
   /// ctor
   Game();

   /// inits game
   void Init();

   /// does tick processing of local model
   void OnTick();

   /// returns local model
   LocalModel& GetModel() { return m_model; }

   /// returns filesystem object
   IFileSystem& GetFileSystem() { return m_fileSystem; }

private:
   /// model
   LocalModel m_model;

   /// tick timer
   Timer m_tickTimer;

   /// time base
   TimeBase m_timeBase;

   /// file system
   VirtualFileSystem m_fileSystem;
};

} // namespace Arena
