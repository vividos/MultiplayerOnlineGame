//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ConsoleClientSession.cpp Console client session
//

// includes
#include "StdAfx.h"
#include "ConsoleClientSession.hpp"

ConsoleClientSession::ConsoleClientSession(boost::asio::io_service& ioService)
:AuthClientSession(ioService),
 m_evtConnectSuccess(true, false) // manual-reset event
{
   SetHandlerOnConnectStateChanged(
      std::bind(&ConsoleClientSession::OnConnectStateChanged, this, std::placeholders::_1));
}

bool ConsoleClientSession::OnReceiveMessage(RawMessage& msg)
{
   if (AuthClientSession::OnReceiveMessage(msg))
      return true;

   if (msgSessionInit == msg.MessageId())
   {
      _tprintf(_T("Session init message\n"));
      return true;
   }
   else
   if (msgPingResponse == msg.MessageId())
   {
      _tprintf(_T("Ping response message\n"));
      return true;
   }
   else
      _tprintf(_T("message received: %04x\n"), msg.MessageId());

   return false;
}

void ConsoleClientSession::OnConnectionClosing()
{
   AuthClientSession::OnConnectionClosing();

   _tprintf(_T("Server logged us out\n"));
}

void ConsoleClientSession::OnConnectStateChanged(ClientSession::T_enConnectState enConnectState)
{
   _tprintf(_T("changed state: %s\n"), ConnectStateToString(enConnectState));

   if (enConnectState == ClientSession::connectStateLoggedIn)
      m_evtConnectSuccess.Set();
}

CString ConsoleClientSession::ConnectStateToString(ClientSession::T_enConnectState enConnectState)
{
   switch (enConnectState)
   {
   case ClientSession::connectStateNotConnected:        return _T("NotConnected"); break;
   case ClientSession::connectStateLookupHostname:      return _T("LookupHostname"); break;
   case ClientSession::connectStateLookupHostnameError: return _T("LookupHostnameError"); break;
   case ClientSession::connectStateConnecting:          return _T("Connecting"); break;
   case ClientSession::connectStateConnectError:        return _T("ConnectError"); break;
   case ClientSession::connectStateConnectSuccess:      return _T("ConnectSuccess"); break;
   case ClientSession::connectStateAuthenticate:        return _T("Authenticate"); break;
   case ClientSession::connectStateAuthFailed:          return _T("AuthFailed"); break;
   case ClientSession::connectStateLoggedIn:            return _T("LoggedIn"); break;
   case ClientSession::connectStateLoggedOut:           return _T("LoggedOut"); break;
   }
   return _T("???");
}
