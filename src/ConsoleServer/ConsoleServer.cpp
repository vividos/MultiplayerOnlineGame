//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ConsoleServer.cpp Console based server
//

// includes
#include "stdafx.h"
#include "ConsoleServer.hpp"
#include "ConsoleServerProgramOptions.hpp"

IMPLEMENT_SINGLETON(ConsoleServer)

void ConsoleServer::Run(const ConsoleServerProgramOptions& opt)
{
   GameServer::SetupLogging();

   // set up ctrl-c handler
   ::SetConsoleCtrlHandler(&ConsoleServer::OnConsoleCtrlC, TRUE);

   LOG_INFO(_T("Multiplayer Online Game - Starting game server"), Log::Server::General);

   try
   {
      m_upGameServer.reset(new GameServer(opt.Port()));

      m_upGameServer->Run();

      m_upGameServer.reset();
   }
   catch (Exception& ex)
   {
      CString cszMessage;
      cszMessage.Format(_T("Exception: %s"), ex.Message());
      LOG_ERROR(cszMessage, Log::Server::General);
   }
   catch (std::exception& ex)
   {
      CString cszMessage;
      cszMessage.Format(_T("std exception: msg:%hs"), ex.what());
      LOG_ERROR(cszMessage, Log::Server::General);
   }
   catch (...)
   {
      CString cszMessage;
      cszMessage.Format(_T("unknown exception"));
      LOG_ERROR(cszMessage, Log::Server::General);
   }

   LOG_INFO(_T("Stopped game server"), Log::Server::General);
}

void ConsoleServer::Stop()
{
   if (m_upGameServer != NULL)
      m_upGameServer->Stop();
}

BOOL WINAPI ConsoleServer::OnConsoleCtrlC(DWORD /*dwCtrlType*/)
{
   LOG_INFO(_T("Console control handler was called"), Log::Server::General);

   ConsoleServer::Instance().Stop();

   return TRUE;
}

/// console main function
int _tmain(int argc, _TCHAR* argv[])
{
   _tprintf(_T("MultiplayerOnlineGame Console Server\n\n"));

   ConsoleServerProgramOptions opt;
   opt.Parse(argc, argv);

   if (opt.IsSelectedHelpOption())
      return 0; // option /help was used

   ConsoleServer::Instance().Run(opt);

   return 0;
}
