//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Common/Object.cpp Object class
//

// includes
#include "StdAfx.h"
#include "Object.hpp"
#include "ByteStream.hpp"

void Object::Serialize(ByteStream& stream) const
{
   stream.WriteUuid(m_id);
   stream.WriteVector(m_pos);
   stream.WriteString(m_cszName);
}

void Object::Deserialize(ByteStream& stream)
{
   m_id = stream.ReadUuid();
   m_pos = stream.ReadVector();
   m_cszName = stream.ReadString(64);
}
