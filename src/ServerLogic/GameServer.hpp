//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file src/ServerLogic/GameServer.hpp game server class
//
#pragma once

// includes
#include "ServerLogic.hpp"
#include <ulib/Event.hpp>
#include "IoServiceThread.hpp"
#include "NetworkManager.hpp"
#include "StaticAccountAuthManager.hpp"
#include "AsyncActionQueue.hpp"
#include "WorldModel.hpp"
#include "WorldRunner.hpp"

/// \brief game server class
/// \details This is the central game server class; it gets instanciated by
/// GameServer.exe as service or ConsoleServer.exe as console application.
class SERVERLOGIC_DECLSPEC GameServer
{
public:
   /// ctor
   GameServer(unsigned short usPort);
   /// dtor
   virtual ~GameServer();

   /// sets up logging
   static void SetupLogging();

   /// runs game server; blocks until Stop() is called
   void Run();

   /// stops game server; may be called from different threads
   void Stop();

private:
   /// event to stop game server
   Event m_evtStop;

   /// event to signal that game server stopped
   Event m_evtStopped;

   // game objects

   /// ioservice thread
   IoServiceThread m_ioService;

   /// authentication manager (must come before network manager)
   StaticAccountAuthManager m_authManager;

   /// session manager
   SessionManager m_sessionManager;

   /// network manager
   NetworkManager m_networkManager;

   /// action queue
   AsyncActionQueue m_actionQueue;

   /// world model
   WorldModel m_worldModel;

   /// world runner
   WorldRunner m_worldRunner;
};
