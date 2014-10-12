//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file src/ServerLogic/GameServer.cpp game server class
//

// includes
#include "StdAfx.h"
#include "GameServer.hpp"
#include <ulib/log/Appender.hpp>
#include <ulib/log/Layout.hpp>
#include "Filesystem.hpp"
#include "Path.hpp"

GameServer::GameServer(unsigned short usPort)
:m_evtStop(true, false), // manual-reset event
 m_evtStopped(true, false), // manual-reset event
 m_sessionManager(m_authManager, m_worldModel, m_ioService.Get()),
 m_networkManager(m_sessionManager, m_ioService.Get(), usPort),
 m_actionQueue(m_ioService.Get(), m_worldModel),
 m_worldRunner(m_worldModel),
 m_worldModel(m_sessionManager, m_actionQueue)
{
   InitDatabase();

   // add static account
   Account a;
   a.Username(_T("michi"));
   a.Password(_T("88D60A34770813E4B5FB7735DBDB1FF1FAEC2F3A6376926D6A72F8CD26BB2ADAE1DA8A41F6B3B019863738C7E9AE168CA1A79A52269619E65D9C234B395A08BC41A5776F0FA92095CFF4A1AAEC9955F274A1C76E547CB300434A12D3E1A0AEA4C37E4C67EC7369A7FF2D5FBACCF1AC4A1B3C3932607F661D2E1A4DB620612D00"));
   a.Salt(_T("26F3DF6AD81481E2761DB9EFE56E3BD8"));
   a.LogonAllowed(true);
   a.AccountId(42);

   m_authManager.AddAccount(a);
}

GameServer::~GameServer()
{
}

void GameServer::SetupLogging()
{
   Log::AppenderPtr spConsoleAppender(new Log::ConsoleAppender);
   Log::AppenderPtr spTraceAppender(new Log::OutputDebugStringAppender);

   Log::LayoutPtr spTraceLayout(new Log::PatternLayout(_T("%F(%L): log [%p] %m")));
   Log::LayoutPtr spConsoleLayout(new Log::PatternLayout(_T("%d %c [%p] %m")));
   spConsoleAppender->Layout(spConsoleLayout);
   spTraceAppender->Layout(spTraceLayout);

   Log::Logger::GetRootLogger()->AddAppender(spConsoleAppender);
   Log::Logger::GetRootLogger()->AddAppender(spTraceAppender);
}

void GameServer::Run()
{
   m_worldRunner.Start();
   m_networkManager.Start();

   // run io service in another thread
   m_ioService.Run();

   // wait for stop signal
   m_evtStop.Wait();

   // now stop all objects that use the IoServiceThread
   m_networkManager.Stop();
   m_worldRunner.Stop();

   m_ioService.Join();

   m_evtStopped.Set();
}

void GameServer::Stop()
{
   m_evtStop.Set();

   m_worldRunner.Stop();

   m_ioService.Get().stop();

   m_evtStopped.Wait();
}

void GameServer::InitDatabase()
{
   CString cszDatabaseFilename = Filesystem().UserFolder() + Database::c_pszDatabaseFilename;

   if (!Path(cszDatabaseFilename).FileExists())
      m_databaseManager.CreateDatabase(cszDatabaseFilename);
   else
      m_databaseManager.OpenExisting(cszDatabaseFilename);
}
