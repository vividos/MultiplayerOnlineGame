//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ServerSession.cpp Server session
//

// includes
#include "stdafx.h"
#include "ServerSession.hpp"
#include "LogoutMessage.hpp"
#include "RawMessage.hpp"
#include "PingMessages.hpp"

void ServerSession::Logout(LogoutMessage::T_enLogoutReason enLogoutReason)
{
   if (!Socket().is_open())
      return;

   LogoutMessage msg(enLogoutReason);
   SendMessage(msg);

   // close socket for reading and writing
   Socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
}

bool ServerSession::OnReceiveMessage(RawMessage& msg)
{
   if (msg.MessageId() == msgPingRequest)
   {
      PingResponseMessage msg;
      SendMessage(msg);
      return true;
   }

   return false;
}
