//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ItemTemplate.hpp Item template class
//
#pragma once

// includes
#include "World.hpp"

// forward references
class ByteStream;

/// weapon item info
class WORLD_DECLSPEC WeaponItemInfo
{
public:
   /// ctor
   WeaponItemInfo()
      :m_enAttackType(attackTypeMelee),
       m_uiAttackSpeed(1),
       m_uiAttackValue(1),
       m_uiAttackVariance(0)
   {
   }

   /// attack type
   enum T_enAttackType
   {
      attackTypeMelee,  ///< melee attack
      attackTypeRange,  ///< ranged attack
   };

   // get methods

   /// returns attack type
   T_enAttackType AttackType() const { return m_enAttackType; }

   /// returns attack speed, in tenths of seconds
   unsigned int AttackSpeed() const { return m_uiAttackSpeed; }

   /// returns attack value
   unsigned int AttackValue() const { return m_uiAttackValue; }

   /// returns attack variance (3 sigma)
   unsigned int AttackVariance() const { return m_uiAttackVariance; }

   // serialize

   /// serialize message by putting bytes to stream
   void Serialize(ByteStream& stream) const;

   /// deserialize message by reading bytes from stream
   void Deserialize(ByteStream& stream);

private:
   /// attack type
   T_enAttackType m_enAttackType;

   /// attack speed, in tenths of seconds
   unsigned int m_uiAttackSpeed;

   /// attack value
   unsigned int m_uiAttackValue;

   /// attack variance (3 sigma)
   unsigned int m_uiAttackVariance;
};

/// armor item info
class WORLD_DECLSPEC ArmorItemInfo
{
public:
   /// ctor
   ArmorItemInfo()
      :m_enArmorType(armorTypeCloth),
       m_uiArmorValue(1)
   {
   }

   /// armor type
   enum T_enArmorType
   {
      armorTypeCloth,   ///< cloth armor
      armorTypeLeather, ///< leather armor
      armorTypeMail,    ///< ring mail armor
      armorTypePlate,   ///< plate armor
   };

   // get methods

   /// returns armor type
   T_enArmorType ArmorType() const { return m_enArmorType; }

   /// returns armor value
   unsigned int ArmorValue() const { return m_uiArmorValue; }

   // serialize

   /// serialize message by putting bytes to stream
   void Serialize(ByteStream& stream) const;

   /// deserialize message by reading bytes from stream
   void Deserialize(ByteStream& stream);

private:
   /// armor type
   T_enArmorType m_enArmorType;

   /// armor value
   unsigned int m_uiArmorValue;
};

/// consumable item info
class WORLD_DECLSPEC ConsumableItemInfo
{
public:
   /// ctor
   ConsumableItemInfo()
   {
   }

   // serialize

   /// serialize message by putting bytes to stream
   void Serialize(ByteStream& stream) const;

   /// deserialize message by reading bytes from stream
   void Deserialize(ByteStream& stream);

private:
};

/// usable item info
class WORLD_DECLSPEC UsableItemInfo
{
public:
   /// ctor
   UsableItemInfo()
   {
   }

   // serialize

   /// serialize message by putting bytes to stream
   void Serialize(ByteStream& stream) const;

   /// deserialize message by reading bytes from stream
   void Deserialize(ByteStream& stream);

private:
};

/// quest item info
class WORLD_DECLSPEC QuestItemInfo
{
public:
   /// ctor
   QuestItemInfo()
   {
   }

   // serialize

   /// serialize message by putting bytes to stream
   void Serialize(ByteStream& stream) const;

   /// deserialize message by reading bytes from stream
   void Deserialize(ByteStream& stream);

private:
};


/// item equip slot type
enum T_enEquipSlotType
{
   equipSlotInvalid = 0,   ///< invalid slot type; item is for inventory only
   equipSlotWeapon,        ///< weapon slot
   equipSlotOffhand,       ///< offhand slot, e.g. shield
   equipSlotArmorHead,     ///< armor piece; head
   equipSlotArmorUpper,    ///< armor piece; upper body
   equipSlotArmorLower,    ///< armor piece; lower body
   equipSlotArmorFeet,     ///< armor piece; feet

   equipSlotCount = equipSlotArmorFeet - 1 ///< count of slots
};


/// item template; a non-instanced item description
class WORLD_DECLSPEC ItemTemplate
{
public:
   /// item type
   enum T_enItemType
   {
      itemTypeWeapon = 0,     ///< weapon object
      itemTypeArmor = 1,      ///< armor object
      itemTypeConsumable = 2, ///< consumable; may restore attribute or trigger effect
      itemTypeUsable = 3,     ///< usable object
      itemTypeQuest = 4,      ///< quest object

      itemTypeMax
   };

   /// item bind type
   enum T_enItemBindType
   {
      bindTypeNone,  ///< no bind type
      bindOnPickup,  ///< bind on pickup
      bindOnEquip,   ///< bind on equip
   };

   /// ctor
   ItemTemplate(unsigned int uiTemplateId, T_enItemType enItemType);

   // get methods

   /// returns item template id
   unsigned int TemplateId() const { return m_uiTemplateId; }

   /// returns item type
   T_enItemType ItemType() const { return m_enItemType; }

   /// returns inventory slot type
   T_enEquipSlotType EquipSlotType() const { return m_enEquipSlotType; }

   /// returns max. durability; 0 for durable item; only set for weapons and armor
   unsigned int MaxDurability() const { return m_uiMaxDurability; }

   /// returns weapon item info
   const WeaponItemInfo& WeaponItem()
   {
      ATLASSERT(m_spWeaponItemInfo != NULL);
      ATLASSERT(m_enItemType == itemTypeWeapon);
      return *m_spWeaponItemInfo;
   }

   /// returns armor item info
   const ArmorItemInfo& ArmorItem()
   {
      ATLASSERT(m_spArmorItemInfo != NULL);
      ATLASSERT(m_enItemType == itemTypeArmor);
      return *m_spArmorItemInfo;
   }

   /// returns consumable item info
   const ConsumableItemInfo& ConsumableItem()
   {
      ATLASSERT(m_spConsumableItemInfo != NULL);
      ATLASSERT(m_enItemType == itemTypeConsumable);
      return *m_spConsumableItemInfo;
   }

   /// returns usable item info
   const UsableItemInfo& UsableItem()
   {
      ATLASSERT(m_spUsableItemInfo != NULL);
      ATLASSERT(m_enItemType == itemTypeUsable);
      return *m_spUsableItemInfo;
   }

   /// returns quest item info
   const QuestItemInfo& QuestItem()
   {
      ATLASSERT(m_spQuestItemInfo != NULL);
      ATLASSERT(m_enItemType == itemTypeQuest);
      return *m_spQuestItemInfo;
   }

   // serialize

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream);

private:
   /// item template id
   unsigned int m_uiTemplateId;

   /// item type
   T_enItemType m_enItemType;

   /// equip slot type
   T_enEquipSlotType m_enEquipSlotType;

   /// max. durability
   unsigned int m_uiMaxDurability;

   /// weapon item info
   std::shared_ptr<WeaponItemInfo> m_spWeaponItemInfo;

   /// armor item info
   std::shared_ptr<ArmorItemInfo> m_spArmorItemInfo;

   /// consumable item info
   std::shared_ptr<ConsumableItemInfo> m_spConsumableItemInfo;

   /// usable item info
   std::shared_ptr<UsableItemInfo> m_spUsableItemInfo;

   /// quest item info
   std::shared_ptr<QuestItemInfo> m_spQuestItemInfo;
};
