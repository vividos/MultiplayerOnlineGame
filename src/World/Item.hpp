//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file Item.hpp Item class
//
#pragma once

// includes
#include "World.hpp"
#include "ItemTemplate.hpp"
#include "Uuid.hpp"

/// instanced (extisting) item
class WORLD_DECLSPEC Item: public ItemTemplate
{
public:
   /// ctor
   Item(const Uuid& id, unsigned int uiItemTeplateId, ItemTemplate::T_enItemType enItemType)
      :ItemTemplate(uiItemTeplateId, enItemType),
       m_id(id),
       m_uiDurability(0)
   {
   }

   /// ctor; constructing item from template
   Item(const Uuid& id, const ItemTemplate& itemTemplate)
      :ItemTemplate(itemTemplate),
       m_id(id),
       m_uiDurability(0)
   {
   }

   // get methods

   /// returns item id
   const Uuid& Id() const throw() { return m_id; }

   /// returns current durability; only set for weapons and armor
   unsigned int Durability() const throw() { return m_uiDurability; }

   // serialize

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream);

private:
   /// unique item id
   Uuid m_id;

   /// current durability
   unsigned int m_uiDurability;
};

/// item smart ptr
typedef std::shared_ptr<Item> ItemPtr;
