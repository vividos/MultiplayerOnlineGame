//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MovePlayerMessage.hpp Move player message
//
#pragma once

// includes
#include "Message.hpp"
#include "MovementInfo.hpp"

// forward references
class ByteStream;

/// message that player wants to move to a new location
class MovePlayerMessage: public Message
{
public:
   /// ctor; use for sending
   MovePlayerMessage(const MovementInfo& movementInfo)
      :Message(msgMovePlayer),
       m_movementInfo(movementInfo)
   {
   }

   /// ctor; use for receiving
   MovePlayerMessage()
      :Message(msgMovePlayer)
   {
   }

   /// dtor
   virtual ~MovePlayerMessage() {}

   // get methods

   /// returns player movement info
   const MovementInfo& Info() const { return m_movementInfo; }

   // serialization

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override
   {
      m_movementInfo.Serialize(stream);
   }

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override
   {
      m_movementInfo.Deserialize(stream);
   }

private:
   /// player movement info
   MovementInfo m_movementInfo;
};
