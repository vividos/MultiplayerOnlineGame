//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file SessionInitMessage.cpp Session init message
//

// includes
#include "StdAfx.h"
#include "SessionInitMessage.hpp"
#include "Mobile.hpp"
#include "Player.hpp"

void SessionInitMessage::Serialize(ByteStream& stream) const
{
   ATLASSERT(m_spPlayer != NULL);

   m_spPlayer->Serialize(stream);

   m_zoneInfo.Serialize(stream);
}

void SessionInitMessage::Deserialize(ByteStream& stream)
{
   ATLASSERT(m_spPlayer == NULL);

   m_spPlayer.reset(new ::Player(ObjectId::Null()));
   m_spPlayer->Deserialize(stream);

   m_zoneInfo.Deserialize(stream);
}
