#include "StdAfx.h"
#include "EquipItemMap.hpp"

EquipItemMap::EquipItemMap()
{
}

void EquipItemMap::PutSlot(T_enEquipSlotType /*enEquipSlotType*/, ItemPtr /*spItem*/)
{
   // TODO impl
}

void EquipItemMap::ClearSlot(T_enEquipSlotType /*enEquipSlotType*/)
{
   // TODO impl
}

ItemPtr EquipItemMap::GetSlot(T_enEquipSlotType /*enEquipSlotType*/) const
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
