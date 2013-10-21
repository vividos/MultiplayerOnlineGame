//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file SessionInitMessage.hpp Message to initialize game session
//
#pragma once

// includes
#include "Network.hpp"
#include "Message.hpp"
#include "ZoneInfo.hpp"

/// initial message that gets sent by server
class NETWORK_DECLSPEC SessionInitMessage : public Message
{
public:
   /// ctor
   SessionInitMessage(MobilePtr spPlayer = MobilePtr())
      :Message(msgSessionInit),
       m_spPlayer(spPlayer)
   {
   }
   /// dtor
   virtual ~SessionInitMessage() throw() {}

   /// returns player object
   MobilePtr Player() throw() { return m_spPlayer; }

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override;

private:
   /// player object
   MobilePtr m_spPlayer;

   /// zone info
   ZoneInfo m_zoneInfo;
};
