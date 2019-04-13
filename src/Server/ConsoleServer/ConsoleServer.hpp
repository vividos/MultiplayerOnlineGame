//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ConsoleServer.hpp Console based server
//
#pragma once

// includes
#include <ulib/Singleton.hpp>
#include "GameServer.hpp"

// forward references
class ConsoleServerProgramOptions;

/// console server
class ConsoleServer: public Singleton<ConsoleServer>
{
public:
   /// runs game server
   void Run(const ConsoleServerProgramOptions& opt);

private:
   friend class Singleton<ConsoleServer>;

   /// ctor
   ConsoleServer()
   {
   }

   /// stops game server
   void Stop();

private:
   /// called when Ctrl+C is pressed
   static BOOL WINAPI OnConsoleCtrlC(DWORD dwCtrlType);

private:
   /// game server instance
   std::unique_ptr<GameServer> m_upGameServer;
};
