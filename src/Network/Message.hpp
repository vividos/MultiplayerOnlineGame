//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Message.hpp Message class
//
#pragma once

// includes

// forward references
class ByteStream;

/// message id
enum T_enMessageId
{
   // misc. messages
   msgNull = 0x0000,          ///< empty message
   msgPingRequest = 0x0001,   ///< ping
   msgPingResponse = 0x0002,  ///< aka pong :)
   msgLogoutRequest = 0x0003, ///< notifies client about logout

   // initial messages
   msgConnectionInit,   ///< sent as first packet from server to client
   msgSessionInit,      ///< sent to init playing session, after authentication

   // messages sent from server to client
   msgAction = 0x0010,        ///< message contains an action to be applied to the model
   msgAddRemoveObject,        ///< message contains an add and remove object message
   msgUpdateObjectMovement,   ///< message contains an update for object movement

   // messages sent from client to server
   msgCommand,       ///< user initiated a command
   msgMovePlayer,    ///< user moved itself

   msgText,          ///< text message

   // simple auth, challenge handshake
   msgAuthBase = 0x0100,
   msgAuthRequest       = msgAuthBase + 0, ///< authenticate request
   msgAuthResponse      = msgAuthBase + 1, ///< authenticate response
   msgAuthAcknowledge   = msgAuthBase + 2, ///< authenticate ack
   msgAuthLogout        = msgAuthBase + 3, ///< logout

   // SRP auth
   msgSRPAuthBase = 0x0200,
   msgSRPAuthRequest    = msgSRPAuthBase + 0, ///< authenticate request
   msgSRPAuthResponse   = msgSRPAuthBase + 1, ///< authenticate response
   msgSRPAuthVerifyRequest = msgSRPAuthBase + 2, ///< authenticate verify request
   msgSRPAuthVerifyResponse = msgSRPAuthBase + 3, ///< authenticate verify response
};

/// single message that is sent between client and server
class Message
{
public:
   /// ctor
   Message(unsigned short usMessageId) throw()
      :m_usMessageId(usMessageId)
   {
   }

   /// dtor
   virtual ~Message() throw() {}

   /// returns message id
   unsigned short MessageId() const throw() { return m_usMessageId; }

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const = 0;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) = 0;

private:
   unsigned short m_usMessageId; ///< message id
};
