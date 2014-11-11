//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file src/GameClient/GameClient.hpp Arena game client
//
#pragma once

// includes
#include "GameClientBase.hpp"
#include "UserInterfaceAudioManager.hpp"
#include "VirtualFileSystem.hpp"

/// game client
class GameClient : public GameClientBase
{
public:
   /// ctor
   GameClient();

   /// starts game client
   void Start();

private:
   /// sets up logging
   void SetupLogging();

private:
   /// UI audio manager
   UserInterfaceAudioManager m_uiAudioManager;

   /// file system
   VirtualFileSystem m_fileSystem;
};
