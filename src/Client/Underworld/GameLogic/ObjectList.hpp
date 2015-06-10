//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/GameLogic/ObjectList.hpp Object list
//
#pragma once

// includes
#include <vector>
#include "Object.hpp"

// forward references
namespace Import
{
class LevelImporter;
}

namespace Underworld
{

/// object position for "no position"
const Uint16 g_uiObjectListPosNone = 0x0000;


/// object list
class ObjectList
{
public:
   /// ctor
   ObjectList(){}

   /// create object list
   void Create();

   /// destroys object list contents
   void Destroy();

   /// allocates a new object list slot
   Uint16 Allocate();

   /// frees object list slot
   void Free(Uint16 uiObjectPos);

   /// returns object from object list position
   ObjectPtr GetObject(Uint16 uiObjectPos);

   /// returns object from object list position; const version
   const ObjectPtr GetObject(Uint16 uiObjectPos) const;

   /// sets new object for given object position
   void SetObject(Uint16 uiObjectPos, const ObjectPtr& pObj);

   /// returns object list start for tile on given coordinates
   Uint16 GetListStart(Uint8 xpos, Uint8 ypos) const;

   /// adds object to list for given tile
   void AddObjectToTileList(Uint16 uiObjectPos, Uint8 xpos, Uint8 ypos);

   /// removes object from list for given tile
   void RemoveObjectFromTileList(Uint16 uiObjectPos, Uint8 xpos, Uint8 ypos);

   // loading / saving

   /// loads object list from savegame
   void Load(UwBase::Savegame& sg);

   /// saves object list to savegame
   void Save(UwBase::Savegame& sg) const;

   /// compacts object list
   void Compact();

private:
   friend class Import::LevelImporter;

   /// sets object list start for tile on given coordinates
   void SetListStart(Uint16 uiObjectPos, Uint8 xpos, Uint8 ypos);

private:
   /// master object list
   std::vector<ObjectPtr> m_objectList;

   /// object list start positions for all tiles in tilemap
   std::vector<Uint16> m_tilemapListStart;
};

} // namespace Underworld
