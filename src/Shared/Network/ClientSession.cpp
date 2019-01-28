//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ClientSession.cpp Client session
//

// includes
#include "stdafx.h"
#include "ClientSession.hpp"
#include "RawMessage.hpp"
#include "LogoutMessage.hpp"
#include "PingMessages.hpp"
#include "AsioHelper.hpp"
#include <boost/bind.hpp>

ClientSession::ClientSession(boost::asio::io_service& ioService)
:Session(ioService),
 m_resolver(ioService),
 m_enConnectState(connectStateNotConnected),
 m_evtSocketConnected(false)
{
}

ClientSession::~ClientSession()
{
   StopConnect();
}

void ClientSession::SetHandlerOnConnectStateChanged(T_fnOnConnectStateChanged fnOnConnectStateChanged)
{
   m_fnOnConnectStateChanged = fnOnConnectStateChanged;
}

void ClientSession::SetHandlerOnLogout(T_fnOnLogout fnOnLogout)
{
   m_fnOnLogout = fnOnLogout;
}

void ClientSession::Connect(const CString& cszServer, unsigned short usPort)
{
   // start determining endpoint
   CString cszService;
   cszService.Format(_T("%u"), usPort);

   USES_CONVERSION;
   boost::asio::ip::tcp::resolver::query query(T2CA(cszServer), T2CA(cszService));

   m_resolver.async_resolve(query, std::bind(&ClientSession::OnAsyncResolve, this, std::placeholders::_1, std::placeholders::_2));

   UpdateConnectState(connectStateLookupHostname);
}

void ClientSession::StopConnect()
{
   m_resolver.cancel();

   if (m_evtSocketConnected.Wait(0))
   {
      boost::asio::ip::tcp::socket& s = Socket();

      boost::system::error_code ec;

      s.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
      s.close(ec);

      m_evtSocketConnected.Reset();

      UpdateConnectState(connectStateNotConnected);
   }
}

void ClientSession::Disconnect()
{
   if (m_evtSocketConnected.Wait(0))
   {
      // we are already connected, so send quit packet
      LogoutMessage msg;
      SendMessage(msg);

      // half-close socket
      boost::asio::ip::tcp::socket& s = Socket();

      boost::system::error_code ec;

      s.shutdown(boost::asio::ip::tcp::socket::shutdown_receive, ec);
      s.close(ec);

      UpdateConnectState(connectStateNotConnected);
   }
   else
      StopConnect();
}

void ClientSession::OnAsyncResolve(const boost::system::error_code& error,
   boost::asio::ip::tcp::resolver::iterator endpointIter)
{
   if (error)
   {
      UpdateConnectState(connectStateLookupHostnameError);
      return;
   }

   boost::asio::ip::tcp::resolver::iterator end; // end marker
   if (endpointIter != end)
   {
      boost::asio::ip::tcp::endpoint e = *endpointIter;

      Socket().async_connect(e,
         boost::bind(&ClientSession::OnConnect, this, boost::asio::placeholders::error, ++endpointIter));

      UpdateConnectState(connectStateConnecting);

      CString cszText = _T("trying endpoint: ") + EndpointToString(e);
      LOG_INFO(cszText, Log::Session);
   }
   else
   {
      // no more endm_points to connect
      UpdateConnectState(connectStateLookupHostnameError);
   }
}

void ClientSession::OnConnect(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpointIter)
{
   if (error)
   {
      // last endpoint?
      if (endpointIter != boost::asio::ip::tcp::resolver::iterator())
      {
         // no, try next
         Socket().close();

         boost::asio::ip::tcp::endpoint e = *endpointIter;
         Socket().async_connect(e,
            boost::bind(&ClientSession::OnConnect, this, boost::asio::placeholders::error, ++endpointIter));

         CString cszText = _T("trying endpoint: ") + EndpointToString(e);
         LOG_INFO(cszText, Log::Session);
         return;
      }

      // other error
      UpdateConnectState(connectStateConnectError);
      return;
   }

   // set TCP_NODELAY option to disable Nagle's Algorithm
   boost::asio::ip::tcp::no_delay option(true);
   Socket().set_option(option);

   // start session
   Session::Start();

   m_evtSocketConnected.Set();

   OnConnectSuccess();

   // must occur after session is set up
   UpdateConnectState(connectStateConnectSuccess);
}

bool ClientSession::OnReceiveMessage(RawMessage& rawMessage)
{
   if (rawMessage.MessageId() == msgPingRequest)
   {
      PingResponseMessage msg;
      SendMessage(msg);
      return true;
   }

   if (rawMessage.MessageId() == msgLogoutRequest)
   {
      LogoutMessage msg;
      ConstVectorRefStream stream(rawMessage.Data());
      msg.Deserialize(stream);

      UpdateConnectState(connectStateLoggedOut);

      if (m_fnOnLogout)
         m_fnOnLogout(msg.LogoutReason());

      Session::Close();

      // TODO needed?
//      throw Exception(_T("bla"), __FILE__, __LINE__);
      return true;
   }

   return false;
}

void ClientSession::OnConnectionClosing()
{
   UpdateConnectState(connectStateLoggedOut);

   if (m_fnOnLogout)
      m_fnOnLogout(LogoutMessage::logoutConnectionDropped);
}

void ClientSession::UpdateConnectState(T_enConnectState enConnectState)
{
   m_enConnectState = enConnectState;

   if (m_fnOnConnectStateChanged != NULL)
      m_fnOnConnectStateChanged(enConnectState);
}
