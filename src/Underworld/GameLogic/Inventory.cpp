//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file  Underworld/GameLogic/Inventory.cpp Inventory implementation
//

// includes
#include "stdafx.h"
#include "Inventory.hpp"
#include "Savegame.hpp"

using Underworld::Inventory;

// Inventory methods

Inventory::Inventory()
:m_uiFloatingObjectPos(c_uiInventorySlotNoItem)
{
}

void Inventory::Create()
{
   m_vecObjectList.resize(0x0100);

   m_uiFloatingObjectPos = c_uiInventorySlotNoItem;
}

void Inventory::Destroy()
{
   m_vecObjectList.clear();
   m_vecSlotList.clear();
   m_vecContainerStack.clear();

   m_uiFloatingObjectPos = c_uiInventorySlotNoItem;
}

Uint16 Inventory::Allocate()
{
   // start after paperdoll objects
   Uint16 uiPos = slotMax;
   Uint16 uiSize = static_cast<Uint16>(m_vecObjectList.size());
   while (uiPos < uiSize && m_vecObjectList[uiPos].m_uiItemID != c_uiItemIDNone)
      uiPos++;

   // hit end of list?
   if (uiPos >= m_vecObjectList.size())
   {
      // already at maximum size?
      if (m_vecObjectList.size() >= 0x10000)
         throw Exception(_T("Error while enlarging inventory list; already at maximum size"), __FILE__, __LINE__);

      // new pos is start of enlarged list
      uiPos = static_cast<Uint16>(m_vecObjectList.size());

      // enlarge list by factor 1,25
      unsigned int uiNewSize = m_vecObjectList.size();
      uiNewSize += uiNewSize >> 2;

      // limit to Uint16 range
      if (uiNewSize >= 0x10000)
         uiNewSize = 0x10000;

      m_vecObjectList.resize(uiNewSize);
   }

   return uiPos;
}

void Inventory::Free(Uint16 uiPos)
{
   UaAssert(uiPos != c_uiInventorySlotNoItem);
   UaAssert(uiPos < m_vecObjectList.size()); // object must not be part of a tile list

   m_vecObjectList[uiPos].m_uiItemID = c_uiItemIDNone;
}

Uint16 Inventory::GetSlotListPos(unsigned int uiIndex) const
{
   UaAssert(uiIndex < GetNumSlots());

   return m_vecSlotList[uiIndex];
}

/// \todo use ObjectProperties to check
bool Inventory::IsContainer(Uint16 uiItemID) const
{
   return uiItemID >= 0x0080 && uiItemID < 0x008f;
}

Uint16 Inventory::GetContainerPos() const
{
   if (m_vecContainerStack.size() == 0)
      return c_uiInventorySlotNoItem; // topmost

   unsigned int uiTopPos = m_vecContainerStack.size()-1;

   UaAssert(true == IsContainer(GetObject(m_vecContainerStack[uiTopPos]).m_uiItemID));
   return m_vecContainerStack[uiTopPos];
}

Uint16 Inventory::GetParentContainerPos() const
{
   if (m_vecContainerStack.size() <= 1)
      return c_uiInventorySlotNoItem; // topmost

   unsigned int uiParentPos = m_vecContainerStack.size()-2;

   UaAssert(true == IsContainer(GetObject(m_vecContainerStack[uiParentPos]).m_uiItemID));
   return m_vecContainerStack[uiParentPos];
}

/// \todo check if object is in current container
void Inventory::OpenContainer(Uint16 uiPos)
{
   UaAssert(true == IsContainer(GetObject(uiPos).m_uiItemID));
   UaAssert(false == GetObject(uiPos).m_bIsQuantity);
   UaAssert(GetContainerPos() != uiPos); // container already open

   // check if user wants to open container from paperdoll
   if (uiPos >= slotPlayerObjectsStart && uiPos < slotMax)
   {
      // clear container stack; container to open becomes topmost
      m_vecContainerStack.clear();
   }

   // quantity field is used as pointer to content of containers
   Uint16 uiLink = GetObject(uiPos).m_uiQuantity;

   BuildSlotList(uiLink);

   // set current container as top item
   m_vecContainerStack.push_back(uiPos);
}

void Inventory::CloseContainer()
{
   UaAssert(m_vecContainerStack.size() > 0);
   UaAssert(true == IsContainer(GetObject(m_vecContainerStack.back()).m_uiItemID));

   Uint16 uiPos = c_uiInventorySlotNoItem; // start of topmost item list at list pos 0

   // remove current container link
   if (m_vecContainerStack.size() > 0)
      m_vecContainerStack.pop_back();

   // get previous topmost item index
   if (m_vecContainerStack.size() > 0)
   {
      uiPos = m_vecContainerStack.back();

      UaAssert(false == GetObject(uiPos).m_bIsQuantity);

      // quantity is used as pointer to content of container
      uiPos = GetObject(uiPos).m_uiQuantity;
   }

   BuildSlotList(uiPos);
}

/// Object must not be member of a container yet.
/// \param[in,out] uiPos position of item to add; if an item is moved, the new
///                position is stored in this parameter.
/// \param uiContainerPos position of container to add item to; when
///        c_uiInventorySlotNoItem is given, it is added to topmost container
/// \return true if object could be added, or false when not (e.g. when
///         container is full; when false is returned, the value of uiPos
///         isn't change.
bool Inventory::AddToContainer(Uint16& uiPos, Uint16 uiContainerPos)
{
   UaAssert(GetObject(uiPos).m_uiLink == 0);

   if (uiContainerPos == c_uiInventorySlotNoItem)
   {
      // adding to topmost container

      // try to find an unused space
      Uint16 ui;
      for (ui=0; ui<8; ui++)
      {
         if (GetObject(ui).m_uiItemID == c_uiItemIDNone)
         {
            // copy object to that slot
            GetObject(ui) = GetObject(uiPos);

            // free object
            Free(uiPos);
            uiPos = ui;
            break;
         }
      }

      if (ui == 8)
         return false; // no space to drop
   }
   else
   {
      UaAssert(true == IsContainer(GetObject(uiContainerPos).m_uiItemID));
      UaAssert(false == GetObject(uiContainerPos).m_bIsQuantity);

      // add to container
      if (GetObject(uiContainerPos).m_uiQuantity == 0)
      {
         // no object in container
         GetObject(uiContainerPos).m_uiQuantity = uiPos;
      }
      else
      {
         // search last object in list
         Uint16 uiLink = GetObject(uiContainerPos).m_uiQuantity;
         Uint16 uiLastLink = uiLink;
         while (uiLink != 0)
         {
            uiLastLink = uiLink;
            uiLink = GetObject(uiLink).m_uiLink;
         }

         GetObject(uiLastLink).m_uiLink = uiPos;
      }
   }

   if (uiContainerPos == GetContainerPos())
   {
      Uint16 uiPos1stObj = uiContainerPos == c_uiInventorySlotNoItem ? c_uiInventorySlotNoItem : GetObject(uiContainerPos).m_uiQuantity;
      BuildSlotList(uiPos1stObj);
   }

   return true;
}

void Inventory::RemoveFromContainer(Uint16 uiPos, Uint16 uiContainerPos)
{
   // if the object to remove is a container, it must not have items in it
   if (IsContainer(GetObject(uiPos).m_uiItemID))
   {
      UaAssert(false == GetObject(uiPos).m_bIsQuantity);
      UaAssert(GetObject(uiPos).m_uiQuantity == 0);
   }

   if (uiContainerPos == c_uiInventorySlotNoItem)
   {
      // remove from topmost container
      UaAssert(uiPos < slotPlayerObjectsStart); // must be in range
      UaAssert(0 == GetObject(uiPos).m_uiLink); // must not have a link

      // as topmost container items don't have m_uiLink set, there is no need
      // to remove linking; just free object
      Free(uiPos);
   }
   else
   {
      UaAssert(false == GetObject(uiContainerPos).m_bIsQuantity);

      // search container for object
      Uint16 uiLink = GetObject(uiContainerPos).m_uiQuantity;
      UaAssert(uiLink != 0); // no objects in container?

      if (uiLink == uiPos)
      {
         // first object in list
         GetObject(uiContainerPos).m_uiQuantity = GetObject(uiLink).m_uiLink;
      }
      else
      {
         // search for object in list
         while (uiLink != 0)
         {
            if (GetObject(uiLink).m_uiLink == uiPos)
            {
               // chain to object after the one to remove
               GetObject(uiLink).m_uiLink = GetObject(uiPos).m_uiLink;
               break;
            }

            uiLink = GetObject(uiLink).m_uiLink;
         }
      }

      GetObject(uiPos).m_uiLink = 0;
   }

   if (uiContainerPos == GetContainerPos())
   {
      Uint16 uiPos1stObj = uiContainerPos == c_uiInventorySlotNoItem ? c_uiInventorySlotNoItem : GetObject(uiContainerPos).m_uiQuantity;
      BuildSlotList(uiPos1stObj);
   }
}

/// \param uiPos position of object to convert to the floating object
/// \return if the given object can be made floated; reasons for not be able
/// \note check weight constraints before making an object floating; an object
///       is considered part of the inventory as soon as it is added as
///       floating object.
/// \todo check if bool return value is needed; not needed, remove it!
bool Inventory::FloatObject(Uint16 uiPos)
{
   UaAssert(m_uiFloatingObjectPos == c_uiInventorySlotNoItem); // must have no floating object
   UaAssert(GetObject(uiPos).m_uiLink == 0); // must not be in a container

   m_uiFloatingObjectPos = uiPos;

   return true;
}

/// Dropping floating objects must support four different scenarios:
/// - dropping objects to the symbol of the currently open container (the parent)
/// - dropping objects to an empty place in the container, basically adding
///   it to the end of the slot list
/// - dropping objects to the paperdoll or paperdoll slots
/// - dropping object to an item in the container; this generates three further
///   possibilities:
///   - when the object is a container, drop item in this container (if possible)
///   - when the objects can be combined, the new object is dropped (there still
///     may be a floating object afterwards, e.g. lighting incense with a torch)
///   - when the objects can't be combined, the objects just swap
/// \param uiContainerPos container pos which should be used to drop item
/// \param uiObjectPos object position in container
/// \return returns if dropping floating item succeeded; reasons for failure is
///         when the container to drop to is full, the object type cannot be
///         dropped in that container, ... TODO
bool Inventory::DropFloatingObject(Uint16 uiContainerPos, Uint16 uiObjectPos)
{
   // add to topmost container / paperdoll?
   if (uiContainerPos == c_uiInventorySlotNoItem)
   {
      // must be a topmost slot or paperdoll pos, or "no item" pos
      UaAssert(uiObjectPos < Underworld::slotMax || uiObjectPos == c_uiInventorySlotNoItem);

      // dropping to topmost slot or paperdoll?
      if (uiObjectPos < Underworld::slotMax)
      {
         // just drop on existing object
         return DropOnObject(uiContainerPos, uiObjectPos);
      }
      else
      {
         // add to topmost container, at end
         bool bRet = AddToContainer(m_uiFloatingObjectPos, uiContainerPos);

         if (bRet)
            m_uiFloatingObjectPos = c_uiInventorySlotNoItem;
         return bRet;
      }
   }
   else
   {
      // no, adding to specified container

      // add to end of container?
      // TODO move together with above code
      if (uiObjectPos == c_uiInventorySlotNoItem)
      {
         bool bRet = AddToContainer(m_uiFloatingObjectPos, uiContainerPos);

         if (bRet)
            m_uiFloatingObjectPos = c_uiInventorySlotNoItem;

         return bRet;
      }
      else
      {
         // swap at specific pos in container
         DropOnObject(uiContainerPos, uiObjectPos);
      }
   }

   return false;
}

bool Inventory::DropOnObject(Uint16 /*uiContainerPos*/, Uint16 /*uiPos*/)
{
   // TODO implement
   UaAssert(false);
   return false;
}

void Inventory::Load(UwBase::Savegame& sg)
{
   Destroy();
   Create();

   sg.BeginSection(_T("inventory"));

   // load inventory object list
   Uint16 uiMax = sg.Read16();
   m_vecObjectList.resize(uiMax);

   for (Uint16 ui=0; ui<uiMax; ui++)
      m_vecObjectList[ui].Load(sg);

   // load container stack
   uiMax = sg.Read16();
   m_vecContainerStack.resize(uiMax);

   for (Uint16 ui2=0; ui2<uiMax; ui2++)
      m_vecContainerStack[ui2] = sg.Read16();

   m_uiFloatingObjectPos = sg.Read16();

   sg.EndSection();

   // rebuild current container list
   BuildSlotList(GetContainerPos());
}

void Inventory::Save(UwBase::Savegame& sg) const
{
   sg.BeginSection(_T("inventory"));

   // save inventory object list
   Uint16 uiMax = static_cast<Uint16>(m_vecObjectList.size());
   sg.Write16(uiMax);

   for (Uint16 ui=0; ui<uiMax; ui++)
      m_vecObjectList[ui].Save(sg);

   // save container stack
   uiMax = static_cast<Uint16>(m_vecContainerStack.size());
   sg.Write16(uiMax);

   for (Uint16 ui2=0; ui2<uiMax; ui2++)
      sg.Write16(m_vecContainerStack[ui2]);

   sg.Write16(m_uiFloatingObjectPos);

   sg.EndSection();
}

/// \param uiPos position of first object to build slot list with. when uiPos
///        is equal to c_uiInventorySlotNoItem, the "top" slot list is built.
void Inventory::BuildSlotList(Uint16 uiPos)
{
   // rebuild slot list
   m_vecSlotList.clear();

   if (uiPos == c_uiInventorySlotNoItem)
   {
      // build list with first 8 items
      for (Uint16 i=0; i<8; i++)
         m_vecSlotList.push_back(i);
   }
   else
   {
      // build normal list
      while (uiPos != 0)
      {
         // add to slot links
         m_vecSlotList.push_back(uiPos);

         // get next object
         uiPos = GetObject(uiPos).m_uiLink;
      }
   }
}
