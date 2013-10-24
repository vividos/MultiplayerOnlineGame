//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2011-2013 Michael Fink
//
//! \file GameServerService.cpp Service class for game server
//

// includes
#include "StdAfx.h"
#include "GameServerService.hpp"
#include "Filesystem.hpp"
#include <ulib/log/Appender.hpp>
#include <ulib/log/Layout.hpp>
#include <ulib/Event.hpp>
#include "ServiceInstance.hpp"

/// service name
LPCTSTR GameServerService::c_pszServiceName = _T("multiplayeronlinegame_server_service");

IMPLEMENT_SERVICE(_T("multiplayeronlinegame_server_service"), GameServerService)

void GameServerService::SetupLogging()
{
   GameServer::SetupLogging();
}

Win32::ServiceInfo GameServerService::GetServiceInfo()
{
   return Win32::ServiceInfo(
      c_pszServiceName,
      _T("Multiplayer Online Game Server"),
      _T("Runs Multiplayer Online Game server"),
      Log::Server::Service,
      Filesystem::ModuleFilename());
}

void GameServerService::Run()
{
   LOG_INFO(_T("Service starting"), Log::Server::Service);

   m_upGameServer.reset(new GameServer(c_usDefaultServerPort));

   m_upGameServer->Run();

   m_upGameServer.reset();

   LOG_INFO(_T("Service has stopped"), Log::Server::Service);
}

void GameServerService::Stop()
{
   LOG_INFO(_T("Service stop requested"), Log::Server::Service);

   m_upGameServer->Stop();
}
