//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file  Underworld/GameLogic/ObjectList.hpp Object list
//

// includes
#include "stdafx.h"
#include "ObjectList.hpp"
#include "Savegame.hpp"

using Underworld::ObjectList;
using Underworld::ObjectPtr;
using Underworld::EObjectType;

// ObjectList methods

void ObjectList::Create()
{
   m_objectList.resize(0x400);
   m_tilemapListStart.resize(64*64, g_uiObjectListPosNone);
}

void ObjectList::Destroy()
{
   m_objectList.clear();
   m_tilemapListStart.clear();
}

/// Allocates a new object by searching next free object position in list. The
/// new object itself isn't set; use SetObject to do that.
/// \note may enlarge object list
/// \todo integrate SetObject() call by passing ObjectPtr
Uint16 ObjectList::Allocate()
{
   // start with position 1; 0 is reserved
   Uint16 uiPos = 1;
   Uint16 uiSize = static_cast<Uint16>(m_objectList.size());
   while (uiPos < uiSize && m_objectList[uiPos].get() != NULL)
      uiPos++;

   // hit end of list?
   if (uiPos >= m_objectList.size())
   {
      // already at maximum size?
      if (m_objectList.size() == 0x10000)
         throw Exception(_T("Error while enlarging object list; already at maximum size"), __FILE__, __LINE__);

      // new pos is start of enlarged list
      uiPos = static_cast<Uint16>(m_objectList.size());

      // enlarge list by factor 1,25
      unsigned int uiNewSize = m_objectList.size();
      uiNewSize += uiNewSize >> 2;

      // limit to Uint16 range
      if (uiNewSize >= 0x10000)
         uiNewSize = 0x10000;

      m_objectList.resize(uiNewSize);
   }

   return uiPos;
}

void ObjectList::Free(Uint16 uiObjectPos)
{
   // object must not be part of a tile list
   ATLASSERT(GetObject(uiObjectPos)->GetPosInfo().m_uiTileX == c_uiTileNotAPos);
   ATLASSERT(GetObject(uiObjectPos)->GetPosInfo().m_uiTileY == c_uiTileNotAPos);
   ATLASSERT(uiObjectPos < m_objectList.size());
   ATLASSERT(uiObjectPos != g_uiObjectListPosNone);
   ATLASSERT(m_objectList[uiObjectPos].get() != NULL); // can only free allocated objects

   m_objectList[uiObjectPos].reset();
}

ObjectPtr ObjectList::GetObject(Uint16 uiObjectPos)
{
   ATLASSERT(uiObjectPos < m_objectList.size());
   ATLASSERT(uiObjectPos != g_uiObjectListPosNone);

   return m_objectList[uiObjectPos];
}

const ObjectPtr ObjectList::GetObject(Uint16 uiObjectPos) const
{
   ATLASSERT(uiObjectPos < m_objectList.size());
   ATLASSERT(uiObjectPos != g_uiObjectListPosNone);

   return m_objectList[uiObjectPos];
}

void ObjectList::SetObject(Uint16 uiObjectPos, const ObjectPtr& pObj)
{
   ATLASSERT(uiObjectPos < m_objectList.size());
   ATLASSERT(uiObjectPos != g_uiObjectListPosNone);

   m_objectList[uiObjectPos] = pObj;
}

Uint16 ObjectList::GetListStart(Uint8 xpos, Uint8 ypos) const
{
   ATLASSERT(xpos < 64);
   ATLASSERT(ypos < 64);

   return m_tilemapListStart[ypos*64+xpos];
}

/// It is allowed that uiObjectPos may be equal to g_uiObjectListPosNone, to
/// empty the list.
void ObjectList::SetListStart(Uint16 uiObjectPos, Uint8 xpos, Uint8 ypos)
{
   ATLASSERT(xpos < 64);
   ATLASSERT(ypos < 64);
   ATLASSERT(uiObjectPos < m_objectList.size());

   m_tilemapListStart[ypos*64+xpos] = uiObjectPos;
}

/// Adds object to tile's list of objects. Adds the object to the end of the list.
void ObjectList::AddObjectToTileList(Uint16 uiObjectPos, Uint8 xpos, Uint8 ypos)
{
   ATLASSERT(uiObjectPos != g_uiObjectListPosNone);
   ATLASSERT(uiObjectPos < m_objectList.size());
   ATLASSERT(m_objectList[uiObjectPos].get() != NULL);
   ATLASSERT(m_objectList[uiObjectPos]->GetObjectInfo().m_uiLink == g_uiObjectListPosNone);

   // search end of tile
   Uint16 uiLink = GetListStart(xpos, ypos);
   if (uiLink == g_uiObjectListPosNone)
   {
      // no object in tile yet
      SetListStart(uiObjectPos, xpos, ypos);
   }
   else
   {
      // follow link to the end
      Uint16 uiLastLink;
      do
      {
         uiLastLink = uiLink;
         uiLink = GetObject(uiLink)->GetObjectInfo().m_uiLink;
      } while(uiLink != g_uiObjectListPosNone);

      // set as new end object
      GetObject(uiLastLink)->GetObjectInfo().m_uiLink = uiObjectPos;
      GetObject(uiObjectPos)->GetObjectInfo().m_uiLink = g_uiObjectListPosNone;
   }

   m_objectList[uiObjectPos]->GetPosInfo().m_uiTileX = xpos;
   m_objectList[uiObjectPos]->GetPosInfo().m_uiTileY = ypos;
}

void ObjectList::RemoveObjectFromTileList(Uint16 uiObjectPos, Uint8 xpos, Uint8 ypos)
{
   ATLASSERT(uiObjectPos != g_uiObjectListPosNone);
   ATLASSERT(uiObjectPos < m_objectList.size());
   ATLASSERT(m_objectList[uiObjectPos].get() != NULL);

   // search item in tile list
   Uint16 uiLink = GetListStart(xpos, ypos);
   ATLASSERT(uiLink != g_uiObjectListPosNone);

   m_objectList[uiObjectPos]->GetPosInfo().m_uiTileX = c_uiTileNotAPos;
   m_objectList[uiObjectPos]->GetPosInfo().m_uiTileY = c_uiTileNotAPos;

   // first item?
   if (uiLink == uiObjectPos)
   {
      // set next item as list start
      SetListStart(GetObject(uiObjectPos)->GetObjectInfo().m_uiLink, xpos, ypos);
      GetObject(uiObjectPos)->GetObjectInfo().m_uiLink = g_uiObjectListPosNone;
      return;
   }

   // follow link until item is found
   Uint16 uiLastLink;
   do
   {
      uiLastLink = uiLink;
      uiLink = GetObject(uiLink)->GetObjectInfo().m_uiLink;

      if (uiLink == uiObjectPos)
      {
         // found; set last object's link to the next one
         GetObject(uiLastLink)->GetObjectInfo().m_uiLink = GetObject(uiLink)->GetObjectInfo().m_uiLink;
         GetObject(uiObjectPos)->GetObjectInfo().m_uiLink = g_uiObjectListPosNone;
         return;
      }

   } while(uiLink != g_uiObjectListPosNone);

   ATLASSERT(false); // when reached here, item didn't belong in this list
}

void ObjectList::Load(UwBase::Savegame& sg)
{
   sg.BeginSection(_T("objectlist"));

   Destroy();
   Create();

   // read in list start positions
   unsigned int ui=0;
   for(ui=0; ui<64*64; ui++)
      m_tilemapListStart[ui] = sg.Read16();

   // read in object list
   Uint16 uiSize = sg.Read16();

   for(ui=0; ui<uiSize; ui++)
   {
      // read in object type
      EObjectType objectType =
         static_cast<EObjectType>(sg.Read8());

      // generate proper object ptr, depending on object type
      ObjectPtr& pObj = m_objectList[ui];
      if (objectType == ::Underworld::objectNone)
         pObj.reset();
      else
      if (objectType == ::Underworld::objectNormal)
         pObj = ObjectPtr(new Object);
      else
      if (objectType == ::Underworld::objectNpc)
         pObj = ObjectPtr(new NpcObject);

      // load contents
      if (pObj.get() != NULL)
         pObj->Load(sg);
   }

   sg.EndSection();
}

void ObjectList::Save(UwBase::Savegame& sg) const
{
   sg.BeginSection(_T("objectlist"));

   // write list with start positions
   unsigned int ui=0;
   for(ui=0; ui<64*64; ui++)
      sg.Write16(m_tilemapListStart[ui]);

   // write out object list
   Uint16 uiSize = static_cast<Uint16>(m_objectList.size());
   sg.Write16(uiSize);

   for(ui=0; ui<uiSize; ui++)
   {
      // write out object type
      const ObjectPtr& pObj = m_objectList[ui];
      if (pObj.get() == NULL)
         sg.Write8(static_cast<Uint8>(::Underworld::objectNone));
      else
      {
         sg.Write8(static_cast<Uint8>(pObj->GetObjectType()));

         // write out object infos, too
         pObj->Save(sg);
      }
   }

   sg.EndSection();
}

/// Compacts object list by rearranging objects and adjusting links and
/// special links; note that no object list positions must be kept, since
/// they will be invalidated after calling this function.
/// \todo implement
void ObjectList::Compact()
{
}
