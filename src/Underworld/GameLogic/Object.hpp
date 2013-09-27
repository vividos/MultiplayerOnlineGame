//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/GameLogic/Object.hpp Object
//
#pragma once

// includes

namespace Underworld
{

/// item id for unused object
const Uint16 c_uiItemIDNone = 0xffff;

/// tile coordinate for "not a position"
const Uint8 c_uiTileNotAPos = 0xff;

// enums

/// object type
enum EObjectType
{
   objectNone,    ///< not an object
   objectNormal=1,///< normal object of type Object
   objectNpc=2,   ///< object of type NpcObject
};

// structs

/// general object info
struct ObjectInfo
{
   /// ctor
   ObjectInfo()
      :m_uiItemID(c_uiItemIDNone),
      m_uiLink(0), // equal to g_uiObjectListPosNone
      m_uiQuality(0),
      m_uiOwner(0),
      m_uiQuantity(0),
      m_uiFlags(0),
      m_bEnchanted(false),
      m_bHidden(false),
      m_bIsQuantity(true)
   {
   }

   Uint16 m_uiItemID;   ///< object item id
   Uint16 m_uiLink;     ///< object chain link
   Uint16 m_uiQuality;  ///< quality
   Uint16 m_uiOwner;    ///< owner / special field
   Uint16 m_uiQuantity; ///< quantity / special / special link
   Uint16 m_uiFlags;    ///< object flags

   // flags
   bool m_bEnchanted;   ///< true when object is enchanted
   bool m_bHidden;      ///< indicates if object is hidden or visible
   bool m_bIsQuantity;  ///< true when "quantity" field is a quantity/special property

   // loading / saving

   /// loads object info from savegame
   void Load(UwBase::Savegame& sg);

   /// saves object info to savegame
   void Save(UwBase::Savegame& sg) const;
};

/// object position info
struct ObjectPositionInfo
{
   /// ctor
   ObjectPositionInfo()
      :m_xpos(0),
      m_ypos(0),
      m_zpos(0),
      m_uiHeading(0),
      m_uiTileX(c_uiTileNotAPos),
      m_uiTileY(c_uiTileNotAPos)
   {
   }

   Uint8 m_xpos;  ///< fractional x position in tile (0..7)
   Uint8 m_ypos;  ///< fractional y position in tile (0..7)
   Uint8 m_zpos;  ///< z position (0..127)
   Uint8 m_uiHeading;   ///< direction (0..7)
   Uint8 m_uiTileX;     ///< tilemap x coordinate
   Uint8 m_uiTileY;     ///< tilemap y coordinate

   // loading / saving

   /// loads object position info from savegame
   void Load(UwBase::Savegame& sg);

   /// saves object position info to savegame
   void Save(UwBase::Savegame& sg) const;
};

/// \brief npc info
/// \details Most members of this struct directly correspond to the variables available in conversations.
struct NpcInfo
{
   /// \todo use enums where values are known and fixed
   NpcInfo()
      :npc_hp(0),
      npc_goal(0),
      npc_gtarg(0),
      npc_level(0),
      npc_talkedto(false),
      npc_attitude(0),
      npc_xhome(0),
      npc_yhome(0),
      npc_hunger(0),
      npc_whoami(0)
   {
   }

   Uint8 npc_hp;        ///< hit points

   Uint8 npc_goal;      ///< goal of the npc
   Uint8 npc_gtarg;     ///< goal target
   Uint8 npc_level;     ///< experience level
   bool  npc_talkedto;  ///< is true when player talked to npc
   Uint8 npc_attitude;  ///< attitude

   Uint8 npc_xhome;     ///< home tile x coordinate
   Uint8 npc_yhome;     ///< home tile y coordinate

   Uint8 npc_hunger;    ///< hungriness of the npc
   Uint8 npc_whoami;    ///< conversation slot of npc, or 0 for generic

   // loading / saving

   /// loads npc info from savegame
   void Load(UwBase::Savegame& sg);

   /// saves npc info to savegame
   void Save(UwBase::Savegame& sg) const;
};

// classes

/// object
class Object
{
public:
   /// ctor
   Object():m_objectType(objectNormal){}

   /// returns object type
   EObjectType GetObjectType() const { return m_objectType; }

   /// returns object info
   ObjectInfo& GetObjectInfo(){ return m_objInfo; }
   /// returns object info
   const ObjectInfo& GetObjectInfo() const { return m_objInfo; }

   /// returns object position info
   ObjectPositionInfo& GetPosInfo(){ return m_posInfo; }
   /// returns object position info
   const ObjectPositionInfo& GetPosInfo() const { return m_posInfo; }

   // loading / saving

   /// loads object from savegame
   virtual void Load(UwBase::Savegame& sg);

   /// saves object to savegame
   virtual void Save(UwBase::Savegame& sg) const;

protected:
   /// object type
   EObjectType m_objectType;

   /// object info
   ObjectInfo m_objInfo;

   /// object position info
   ObjectPositionInfo m_posInfo;
};

/// \brief Object that additionally has NPC infos
/// Allowed range for NPC objects is item IDs 0x0040-0x007f.
class NpcObject: public Object
{
public:
   /// ctor
   NpcObject(){ m_objectType = objectNpc; }

   /// returns npc info
   NpcInfo& GetNpcInfo(){ return m_npcInfo; }
   /// returns npc info
   const NpcInfo& GetNpcInfo() const { return m_npcInfo; }

   // loading / saving

   /// loads object from savegame
   virtual void Load(UwBase::Savegame& sg);

   /// saves object to savegame
   virtual void Save(UwBase::Savegame& sg) const;

private:
   /// npc info
   NpcInfo m_npcInfo;
};


/// smart pointer to Object
typedef std::shared_ptr<Object> ObjectPtr;


/// returns if given object is a NPC object
inline bool IsNpcObject(const ObjectPtr& ptr)
{
   return ptr->GetObjectType() == objectNpc;
}

/// \brief Casts object ptr to npc object
/// \details Check pointer with IsNpcObject() before attempting to cast.
inline NpcObject& CastToNpcObject(ObjectPtr& ptr)
{
   ATLASSERT(IsNpcObject(ptr) == true);
   return *reinterpret_cast<NpcObject*>(ptr.get());
}

/// \brief Casts object ptr to npc object
/// \details Check pointer with IsNpcObject() before attempting to cast.
inline const NpcObject& CastToNpcObject(const ObjectPtr& ptr)
{
   ATLASSERT(IsNpcObject(ptr) == true);
   return *reinterpret_cast<const NpcObject*>(ptr.get());
}

} // namespace Underworld
