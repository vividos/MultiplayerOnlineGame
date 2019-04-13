//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SocketListener.hpp Socket listener
//
#pragma once

// includes
#include "Network.hpp"

// forward references
class InstanceManager;
class ISessionManager;
class Session;

/// listens on socket and starts new server session on connect
class NETWORK_DECLSPEC SocketListener: public boost::noncopyable
{
public:
   /// ctor
   SocketListener(ISessionManager& sessionManager, boost::asio::io_service& ioService, bool bUseIPv6, unsigned short usPort);
   /// dtor
   ~SocketListener();

   /// handles accepting 
   void HandleAccept(std::shared_ptr<Session> spSession,
      const boost::system::error_code& error);

   /// stops socket listener
   void Stop();

private:
   /// io service ref
   boost::asio::io_service& m_ioService;

   /// acceptor
   boost::asio::ip::tcp::acceptor m_acceptor;

   /// instance manager ref
   ISessionManager& m_sessionManager;

   /// is IPv6 listener?
   bool m_bIPv6Listener;
};
