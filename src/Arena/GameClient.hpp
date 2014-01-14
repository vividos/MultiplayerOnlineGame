//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Arena/GameClient.hpp Arena game client
//
#pragma once

// includes
#include "GameClientBase.hpp"
#include "Game.hpp"
#include "UserInterfaceAudioManager.hpp"

namespace Arena
{

/// Arena game client
class GameClient: public GameClientBase
{
public:
   /// ctor
   GameClient();
   /// dtor
   virtual ~GameClient();

   /// starts game client
   void Start();

private:
   /// sets up logging
   void SetupLogging();

   /// tick processing
   virtual void OnTick() override;

private:
   /// game object
   Arena::Game m_game;

   /// UI audio manager
   UserInterfaceAudioManager m_uiAudioManager;
};

} // namespace Arena
