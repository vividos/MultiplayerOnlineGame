//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file GameServerService.hpp Service class for game server
//
#pragma once

// include
#include "GameServer.hpp"
#include "ServiceInfo.hpp"

/// game server service
class GameServerService
{
public:
   /// ctor
   GameServerService() throw()
   {
   }

   /// sets up logging
   static void SetupLogging();

   /// returns service info
   static Win32::ServiceInfo GetServiceInfo();

   /// runs game server
   void Run();

   /// stops game server
   void Stop();

private:
   /// service name
   static LPCTSTR c_pszServiceName;

   /// game server object
   std::unique_ptr<GameServer> m_upGameServer;
};
