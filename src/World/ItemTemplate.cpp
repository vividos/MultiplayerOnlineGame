//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file ItemTemplate.cpp Item template class
//

// includes
#include "StdAfx.h"
#include "ItemTemplate.hpp"
#include "ByteStream.hpp"

// WeaponItemInfo methods

void WeaponItemInfo::Serialize(ByteStream& /*stream*/) const
{
   // TODO impl
}

void WeaponItemInfo::Deserialize(ByteStream& /*stream*/)
{
   // TODO impl
}

// ArmorItemInfo methods

void ArmorItemInfo::Serialize(ByteStream& /*stream*/) const
{
   // TODO impl
}

void ArmorItemInfo::Deserialize(ByteStream& /*stream*/)
{
   // TODO impl
}

// ConsumableItemInfo methods

void ConsumableItemInfo::Serialize(ByteStream& /*stream*/) const
{
}

void ConsumableItemInfo::Deserialize(ByteStream& /*stream*/)
{
}

// UsableItemInfo methods

void UsableItemInfo::Serialize(ByteStream& /*stream*/) const
{
}

void UsableItemInfo::Deserialize(ByteStream& /*stream*/)
{
}

// QuestItemInfo methods

void QuestItemInfo::Serialize(ByteStream& /*stream*/) const
{
}

void QuestItemInfo::Deserialize(ByteStream& /*stream*/)
{
}


// ItemTemplate methods

ItemTemplate::ItemTemplate(unsigned int uiTemplateId, T_enItemType enItemType)
:m_uiTemplateId(uiTemplateId),
 m_enItemType(enItemType),
 m_enEquipSlotType(equipSlotInvalid),
 m_uiMaxDurability(0)
{
   switch (enItemType)
   {
   case itemTypeWeapon:    m_spWeaponItemInfo.reset(new WeaponItemInfo); break;
   case itemTypeArmor:     m_spArmorItemInfo.reset(new ArmorItemInfo); break;
   case itemTypeConsumable:m_spConsumableItemInfo.reset(new ConsumableItemInfo); break;
   case itemTypeUsable:    m_spUsableItemInfo.reset(new UsableItemInfo); break;
   case itemTypeQuest:     m_spQuestItemInfo.reset(new QuestItemInfo); break;
   }
}

void ItemTemplate::Serialize(ByteStream& stream) const
{
   stream.Write32(m_uiTemplateId);

   stream.Write8(static_cast<unsigned char>(m_enItemType) & 0xff);

   stream.Write8(static_cast<unsigned char>(m_enEquipSlotType) & 0xff);

   // TODO

   switch (m_enItemType)
   {
   case itemTypeWeapon:    m_spWeaponItemInfo->Serialize(stream); break;
   case itemTypeArmor:     m_spArmorItemInfo->Serialize(stream); break;
   case itemTypeConsumable:m_spConsumableItemInfo->Serialize(stream); break;
   case itemTypeUsable:    m_spUsableItemInfo->Serialize(stream); break;
   case itemTypeQuest:     m_spQuestItemInfo->Serialize(stream); break;
   }
}

void ItemTemplate::Deserialize(ByteStream& stream)
{
   m_uiTemplateId = stream.Read32();

   {
      unsigned char ucItemType = stream.Read8();
      if (ucItemType >= itemTypeMax)
         throw Exception(_T("invalid item type"), __FILE__, __LINE__);

      m_enItemType = static_cast<T_enItemType>(ucItemType);
   }

   {
      unsigned char ucEquipSlotType = stream.Read8();
      if (equipSlotCount > itemTypeMax)
         throw Exception(_T("invalid equip slot type"), __FILE__, __LINE__);

      m_enEquipSlotType = static_cast<T_enEquipSlotType>(ucEquipSlotType);
   }

   // TODO impl
}
