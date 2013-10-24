//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file NetworkManager.cpp Network manager class
//

// includes
#include "stdafx.h"
#include "NetworkManager.hpp"

/// client's server session cleanup interval, in seconds
const unsigned int c_uiCleanupClientIntervalSec = 10;

NetworkManager::NetworkManager(IAuthManager& authManager, boost::asio::io_service& ioService, unsigned short usPort)
:m_sessionManager(authManager, ioService),
 m_socketListenerIPv4(m_sessionManager, ioService, false, usPort),
 m_socketListenerIPv6(m_sessionManager, ioService, true, usPort),
 m_timerCleanupClients(ioService),
 m_evtTimerEnded(true, false) // manual-reset event
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
