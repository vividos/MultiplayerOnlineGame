//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file CommandMessage.hpp Command message class
//
#pragma once

// includes
#include "Command.hpp"
#include "Message.hpp"

/// message that player wants to initiate a Command
class CommandMessage: public Message
{
public:
   /// ctor, use for receiving message
   CommandMessage()
      :Message(msgCommand),
       m_cmd(cmdNull, ObjectId::Null(), ObjectId::Null())
   {
   }
   /// ctor, use for sending message
   CommandMessage(Command& cmd)
      :Message(msgCommand),
       m_cmd(cmd)
   {
   }
   /// dtor
   virtual ~CommandMessage() {}

   /// returns command
   Command& GetCommand() { return m_cmd; }

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override
   {
      m_cmd.Serialize(stream);
   }

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override
   {
      m_cmd.Deserialize(stream);
   }

private:
   /// command to send/receive
   Command m_cmd;
};
