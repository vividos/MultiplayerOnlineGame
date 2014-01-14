//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file GameServer.cpp Game server main function
//

// includes
#include "StdAfx.h"
#include "GameServerService.hpp"
#include "SimpleServiceRunner.hpp"

/// server main function
int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
   GameServerService::SetupLogging();

   try
   {
      Win32::SimpleServiceRunner<GameServerService> runner;
      runner.Run();
   }
   catch(Exception& ex)
   {
      LOG_FATAL(_T("Service ended with exception: ") + ex.Message(), Log::Server::Service);
   }
   catch(...)
   {
      LOG_FATAL(_T("Service ended with unknown exception"), Log::Server::Service);
   }

   return 0;
}
