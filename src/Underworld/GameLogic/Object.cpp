//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/GameLogic/Object.cpp Object
//

// includes
#include "stdafx.h"
#include "Object.hpp"
#include "Savegame.hpp"

using Underworld::ObjectInfo;
using Underworld::ObjectPositionInfo;
using Underworld::NpcInfo;
using Underworld::Object;
using Underworld::NpcObject;

// ObjectInfo methods

void ObjectInfo::Load(UwBase::Savegame& sg)
{
   // read basic object info
   m_uiItemID = sg.Read16();

   if (m_uiItemID == c_uiItemIDNone)
      return; // nothing more to read

   m_uiLink = sg.Read16();
   m_uiQuality = sg.Read16();
   m_uiOwner = sg.Read16();
   m_uiQuantity = sg.Read16();

   m_uiFlags = sg.Read16();

   m_bEnchanted = sg.Read8() != 0;
   m_bHidden = sg.Read8() != 0;
   m_bIsQuantity = sg.Read8() != 0;
}

void ObjectInfo::Save(UwBase::Savegame& sg) const
{
   // write basic object info
   sg.Write16(m_uiItemID);

   if (m_uiItemID == c_uiItemIDNone)
      return; // nothing more to write

   sg.Write16(m_uiLink);
   sg.Write16(m_uiQuality);
   sg.Write16(m_uiOwner);
   sg.Write16(m_uiQuantity);

   sg.Write16(m_uiFlags);

   sg.Write8(m_bEnchanted ? 1 : 0);
   sg.Write8(m_bHidden ? 1 : 0);
   sg.Write8(m_bIsQuantity ? 1 : 0);
}

// ObjectPositionInfo methods

void ObjectPositionInfo::Load(UwBase::Savegame& sg)
{
   // read extended object info
   m_xpos = sg.Read8() & 7;
   m_ypos = sg.Read8() & 7;
   m_zpos = sg.Read8();
   m_uiHeading = sg.Read8() & 7;

   m_uiTileX = sg.Read8();
   m_uiTileY = sg.Read8();
}

void ObjectPositionInfo::Save(UwBase::Savegame& sg) const
{
   sg.Write8(m_xpos & 7);
   sg.Write8(m_ypos & 7);
   sg.Write8(m_zpos);
   sg.Write8(m_uiHeading & 7);

   sg.Write8(m_uiTileX);
   sg.Write8(m_uiTileY);
}

// NpcInfo methods

void NpcInfo::Load(UwBase::Savegame& sg)
{
   npc_hp = sg.Read8();

   npc_goal = sg.Read8();
   npc_gtarg = sg.Read8();
   npc_level = sg.Read8();
   npc_talkedto = sg.Read8() != 0;
   npc_attitude = sg.Read8();

   npc_xhome = sg.Read8();
   npc_yhome = sg.Read8();

   npc_hunger = sg.Read8();
   npc_whoami = sg.Read8();
}

void NpcInfo::Save(UwBase::Savegame& sg) const
{
   sg.Write8(npc_hp);

   sg.Write8(npc_goal);
   sg.Write8(npc_gtarg);
   sg.Write8(npc_level);
   sg.Write8(npc_talkedto ? 1 : 0);
   sg.Write8(npc_attitude);

   sg.Write8(npc_xhome);
   sg.Write8(npc_yhome);

   sg.Write8(npc_hunger);
   sg.Write8(npc_whoami);
}

// Object methods

void Object::Load(UwBase::Savegame& sg)
{
   m_objInfo.Load(sg);
   m_posInfo.Load(sg);
}

void Object::Save(UwBase::Savegame& sg) const
{
   m_objInfo.Save(sg);
   if (m_objInfo.m_uiItemID != c_uiItemIDNone)
      m_posInfo.Save(sg);
}

void NpcObject::Load(UwBase::Savegame& sg)
{
   Object::Load(sg);
   if (m_objInfo.m_uiItemID != c_uiItemIDNone)
      m_npcInfo.Load(sg);
}

void NpcObject::Save(UwBase::Savegame& sg) const
{
   Object::Save(sg);
   m_npcInfo.Save(sg);
}
