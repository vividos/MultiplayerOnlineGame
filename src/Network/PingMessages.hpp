//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file PingMessages.hpp Ping message classes
//
#pragma once

// includes
#include "Message.hpp"
#include "ByteStream.hpp"

/// ping message
class PingRequestMessage: public Message
{
public:
   /// request value
   static const unsigned char c_bRequestValue = 42;

   /// ctor
   PingRequestMessage()
      :Message(msgPingRequest)
   {
   }

   // serialize

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override
   {
      stream.Write8(c_bRequestValue);
   }

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override
   {
      BYTE bData = stream.Read8();
      if (bData != c_bRequestValue)
         throw Exception(_T("invalid ping request"), __FILE__, __LINE__);
   }
};


/// ping response
class PingResponseMessage: public Message
{
public:
   /// response value
   static const unsigned char c_bResponseValue = 64;

   /// ctor
   PingResponseMessage()
      :Message(msgPingResponse)
   {
   }

   // serialize

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override
   {
      stream.Write8(c_bResponseValue);
   }

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override
   {
      BYTE bData = stream.Read8();
      if (bData != c_bResponseValue)
         throw Exception(_T("invalid ping response"), __FILE__, __LINE__);
   }
};
