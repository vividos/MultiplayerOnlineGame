#include "StdAfx.h"
#include "EquipItemMap.hpp"

EquipItemMap::EquipItemMap() throw()
{
}

void EquipItemMap::PutSlot(T_enEquipSlotType /*enEquipSlotType*/, ItemPtr /*spItem*/) throw()
{
   // TODO impl
}

void EquipItemMap::ClearSlot(T_enEquipSlotType /*enEquipSlotType*/) throw()
{
   // TODO impl
}

ItemPtr EquipItemMap::GetSlot(T_enEquipSlotType /*enEquipSlotType*/) const throw()
{
   // TODO impl
   return ItemPtr();
}

void EquipItemMap::Serialize(ByteStream& /*stream*/) const
{
   // TODO impl
}

void EquipItemMap::Deserialize(ByteStream& /*stream*/)
{
   // TODO impl
}
