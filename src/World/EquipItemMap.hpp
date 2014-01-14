//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file Item.hpp Item class
//
#pragma once

// includes
#include "Item.hpp"

/// mapping of equipped items
class EquipItemMap
{
public:
   /// ctor
   EquipItemMap() throw();

   // actions

   /// put item in slot
   void PutSlot(T_enEquipSlotType enEquipSlotType, ItemPtr spItem) throw();

   /// clear slot
   void ClearSlot(T_enEquipSlotType enEquipSlotType) throw();

   /// returns item in slot
   ItemPtr GetSlot(T_enEquipSlotType enEquipSlotType) const throw();

   // serialize

   /// serialize message by putting bytes to stream
   void Serialize(ByteStream& stream) const;

   /// deserialize message by reading bytes from stream
   void Deserialize(ByteStream& stream);

private:
   /// map type for equipped items map
   typedef std::map<T_enEquipSlotType, ItemPtr> T_mapEquipItems;

   /// equipped items map
   T_mapEquipItems m_mapEquipItems;
};
