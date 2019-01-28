//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file NetworkManager.hpp Network manager class
//
#pragma once

// includes
#include "SocketListener.hpp"
#include <ulib/thread/Event.hpp>

// forward references
class SessionManager;

/// \brief network manager
/// \details manages the network side of running the game server
class NetworkManager
{
public:
   /// ctor
   NetworkManager(SessionManager& sessionManager, boost::asio::io_service& ioService, unsigned short usPort);

   /// starts network manager
   void Start();

   /// stops network manager
   void Stop();

private:
   /// (re)starts timer to clean up clients
   void RestartTimer();

   /// timer handler to clean up clients
   void OnTimerCleanupClients(const boost::system::error_code& error);

private:
   /// session manager
   SessionManager& m_sessionManager;

   /// IPv4 socket listener
   SocketListener m_socketListenerIPv4;

   /// IPv6 socket listener
   SocketListener m_socketListenerIPv6;

   /// deadline timer to clean up clients
   boost::asio::deadline_timer m_timerCleanupClients;

   /// event to signal ended timer
   ManualResetEvent m_evtTimerEnded;
};
