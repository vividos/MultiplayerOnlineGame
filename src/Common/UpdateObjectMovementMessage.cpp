//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file UpdateObjectMovementMessage.cpp Object movement update message
//

// includes
#include "StdAfx.h"
#include "UpdateObjectMovementMessage.hpp"
#include "ByteStream.hpp"

void UpdateObjectMovementMessage::Serialize(ByteStream& stream) const
{
   stream.WriteUuid(m_objId);

   m_movementInfo.Serialize(stream);
}

void UpdateObjectMovementMessage::Deserialize(ByteStream& stream)
{
   m_objId = stream.ReadUuid();

   m_movementInfo.Deserialize(stream);
}
