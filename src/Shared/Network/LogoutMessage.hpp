//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file LogoutMessage.hpp Logout message class
//
#pragma once

// includes
#include "Message.hpp"
#include "ByteStream.hpp"

/// logout message; sent by client or server
class LogoutMessage: public Message
{
public:
   /// logout reason
   enum T_enLogoutReason
   {
      logoutUserPassUnknown = 0, ///< unknown user or wrong password
      logoutServerShutdown = 1,  ///< server is being shutdown
      logoutBlocked = 2,         ///< logged out due to blocked account
      logoutConnectionDropped = 3,  ///< logout due to connection being dropped
      logoutServerFull = 4,      ///< full server
      logoutMaintenance = 5,     ///< maintenance cycle on server
      logoutOther = 6,           ///< other reason
   };

   /// ctor
   LogoutMessage(T_enLogoutReason enLogoutReason = logoutOther)
      :Message(msgLogoutRequest),
       m_enLogoutReason(enLogoutReason)
   {
   }

   /// returns logout reason
   T_enLogoutReason LogoutReason() const { return m_enLogoutReason; }

   // serialization

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override
   {
      stream.Write8(static_cast<unsigned char>(m_enLogoutReason));
   }

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override
   {
      m_enLogoutReason = static_cast<T_enLogoutReason>(stream.Read8());
   }

private:
   /// logout reason
   T_enLogoutReason m_enLogoutReason;
};
