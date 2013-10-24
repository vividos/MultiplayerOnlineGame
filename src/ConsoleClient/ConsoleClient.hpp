//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ConsoleClient.hpp Console based client
//
#pragma once

// includes
//#include "InstanceManager.hpp"
#include "IoServiceThread.hpp"
#include "ClientSession.hpp"

// forward references
class ConsoleClientSession;

/// console game client
class ConsoleClient
{
public:
   /// ctor
   ConsoleClient();

   /// runs client
   void Run();

private:
   /// sets up logging
   void SetupLogging();

   /// starts authentication
   void StartConnect();

   /// waits for connection
   void WaitForConnect(bool& bConnect);

   /// runs command loop
   void RunCommandLoop();

private:
   /// io service thread
   IoServiceThread m_ioService;

   /// instance manager
   //InstanceManager m_instangeManager;

   /// session
   std::shared_ptr<ConsoleClientSession> m_spSession;
};
