//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ServerSession.hpp Server session
//
#pragma once

// includes
#include "Network.hpp"
#include "Session.hpp"
#include "LogoutMessage.hpp"

/// \brief server session
/// \details Server session for one client; manages sending/receiving messages to/from client.
class NETWORK_DECLSPEC ServerSession: public Session
{
public:
   /// ctor
   ServerSession(boost::asio::io_service& ioService)
      :Session(ioService)
   {
   }
   /// dtor
   virtual ~ServerSession() throw() {}

   /// logs out session
   void Logout(LogoutMessage::T_enLogoutReason enLogoutReason);

protected:
   virtual bool OnReceiveMessage(RawMessage& msg) override;
};
