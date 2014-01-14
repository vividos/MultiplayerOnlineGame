//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Item.cpp Item class
//

// includes
#include "StdAfx.h"
#include "Item.hpp"
#include "ByteStream.hpp"

void Item::Serialize(ByteStream& stream) const
{
   ItemTemplate::Serialize(stream);

   stream.WriteUuid(m_id);

   if (m_uiDurability > ItemTemplate::MaxDurability() || m_uiDurability > 0xff)
      throw Exception(_T("invalid durability value"), __FILE__, __LINE__);

   stream.Write8(m_uiDurability & 0xff);
}

void Item::Deserialize(ByteStream& stream)
{
   ItemTemplate::Deserialize(stream);

   m_id = stream.ReadUuid();

   m_uiDurability = stream.Read8();
   if (m_uiDurability > ItemTemplate::MaxDurability())
      throw Exception(_T("invalid durability value"), __FILE__, __LINE__);
}
