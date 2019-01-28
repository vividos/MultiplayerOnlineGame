//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file NetworkManager.cpp Network manager class
//

// includes
#include "stdafx.h"
#include "NetworkManager.hpp"
#include "SessionManager.hpp"

/// client's server session cleanup interval, in seconds
const unsigned int c_uiCleanupClientIntervalSec = 10;

NetworkManager::NetworkManager(SessionManager& sessionManager,
   boost::asio::io_service& ioService, unsigned short usPort)
:m_sessionManager(sessionManager),
 m_socketListenerIPv4(sessionManager, ioService, false, usPort),
 m_socketListenerIPv6(sessionManager, ioService, true, usPort),
 m_timerCleanupClients(ioService),
 m_evtTimerEnded(false)
{
}

void NetworkManager::Start()
{
   RestartTimer();
}

void NetworkManager::Stop()
{
   m_timerCleanupClients.cancel();

   m_socketListenerIPv4.Stop();
   m_socketListenerIPv6.Stop();

   m_sessionManager.LogoutAll();

   m_evtTimerEnded.Wait();
}

void NetworkManager::RestartTimer()
{
   // start timer to clean up clients
   m_timerCleanupClients.expires_from_now(boost::posix_time::seconds(c_uiCleanupClientIntervalSec));
   m_timerCleanupClients.async_wait(std::bind(
      &NetworkManager::OnTimerCleanupClients, this, std::placeholders::_1));
}

void NetworkManager::OnTimerCleanupClients(const boost::system::error_code& error)
{
   if (error)
   {
      m_evtTimerEnded.Set();
      return;
   }

   // do cleanup
   m_sessionManager.Cleanup();

   RestartTimer();
}
