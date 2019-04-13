//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ConsoleClientSession.hpp Console client session
//
#pragma once

// includes
#include "RawMessage.hpp"
#include "AuthClientSession.hpp"

/// session for console client
class ConsoleClientSession: public AuthClientSession
{
public:
   /// ctor
   ConsoleClientSession(boost::asio::io_service& ioService);

   /// ctor
   virtual ~ConsoleClientSession() {}

   /// returns event to signal connect success
   ManualResetEvent& GetConnectSuccessEvent()
   {
      return m_evtConnectSuccess;
   }

private:
   /// called when message is being received
   virtual bool OnReceiveMessage(RawMessage& msg) override;

   /// called when connection is closing
   virtual void OnConnectionClosing() override;

   /// called when connect state changes
   void OnConnectStateChanged(ClientSession::T_enConnectState enConnectState);

   /// converts connect state to text
   static CString ConnectStateToString(ClientSession::T_enConnectState enConnectState);

private:
   /// event to signal connect success
   ManualResetEvent m_evtConnectSuccess;
};
