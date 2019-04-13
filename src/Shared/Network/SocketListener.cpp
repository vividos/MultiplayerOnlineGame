//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SocketListener.cpp Socket listener
//

// includes
#include "stdafx.h"
#include "SocketListener.hpp"
#include "ISessionManager.hpp"
#include "Session.hpp"
#include "AsioHelper.hpp"
#include <boost/bind.hpp>

SocketListener::SocketListener(ISessionManager& sessionManager, boost::asio::io_service& ioService, bool bUseIPv6, unsigned short usPort)
:m_sessionManager(sessionManager),
 m_ioService(ioService),
 m_acceptor(ioService, boost::asio::ip::tcp::endpoint(bUseIPv6 ? boost::asio::ip::tcp::v6() : boost::asio::ip::tcp::v4(), usPort)),
 m_bIPv6Listener(bUseIPv6)
{
   if (bUseIPv6 && !IsAvailIPv6())
   {
      LOG_WARN(_T("IPv6 disabled, not starting up socket listener"), Log::Server::General);
      return;
   }

   CString cszMessage;
   cszMessage.Format(_T("Start listening on port %u (IPv%u)"), usPort, bUseIPv6 ? 6 : 4);
   LOG_INFO(cszMessage, Log::Server::General);

   std::shared_ptr<Session> spSession = sessionManager.CreateNewSession();

   // wait for next incoming connection
   m_acceptor.async_accept(spSession->Socket(),
      boost::bind(&SocketListener::HandleAccept, this, spSession,
         boost::asio::placeholders::error));
}

SocketListener::~SocketListener()
{
   try
   {
      Stop();
   }
   catch(...)
   {
      LOG_ERROR(_T("Error while stopping listener in dtor"), Log::Server::General);
   }
}

void SocketListener::HandleAccept(std::shared_ptr<Session> spSession,
   const boost::system::error_code& error)
{
   if (!error)
   {
      boost::asio::ip::tcp::endpoint remoteEndpoint = spSession->Socket().remote_endpoint();

      CString cszMessage;
      cszMessage.Format(_T("Incoming connection from %s (IPv%u)"),
         EndpointToString(remoteEndpoint).GetString(),
         remoteEndpoint.protocol() == boost::asio::ip::tcp::v4() ? 4 : 6);

      LOG_INFO(cszMessage, Log::Server::General);

      // set TCP_NODELAY option to disable Nagle's Algorithm
      boost::asio::ip::tcp::no_delay option(true);
      spSession->Socket().set_option(option);

      spSession->Start();

      // overwrites spSession
      spSession = m_sessionManager.CreateNewSession();

      // wait for next incoming connection
      m_acceptor.async_accept(spSession->Socket(),
         boost::bind(&SocketListener::HandleAccept, this, spSession,
            boost::asio::placeholders::error));
   }
   else if (error.value() == boost::asio::error::operation_aborted)
   {
      CString cszText;
      cszText.Format(_T("Stopping accepting connections (IPv%u)"),
         m_bIPv6Listener ? 4 : 6);

      LOG_INFO(cszText, Log::Server::General);
   }
   else
   {
      CString cszText;
      cszText.Format(_T("Error while accepting connection; code=%u"), error.value());
      LOG_ERROR(cszText, Log::Server::General);
   }
}

void SocketListener::Stop()
{
   boost::system::error_code ec;
   m_acceptor.close(ec);
}
