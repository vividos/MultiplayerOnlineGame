//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/GameLogic/Player.cpp Player
//

// includes
#include "stdafx.h"
#include "Player.hpp"
#include "Savegame.hpp"

using Underworld::Player;

// Player methods

Player::Player()
:m_cszName("GRONKEY"),
 m_xpos(32.0), m_ypos(32.0),
 m_dHeight(0.0),
 m_dRotateAngle(0.0),
 m_dPanAngle(0.0)
{
   memset(m_aAttributes, 0, SDL_TABLESIZE(m_aAttributes));
   memset(m_aSkills, 0, SDL_TABLESIZE(m_aSkills));
}

void Player::Load(UwBase::Savegame& sg)
{
   sg.BeginSection(_T("player"));

   // read in name
   sg.ReadString(m_cszName);

   // read position
   m_xpos = sg.Read32() / 256.0;
   m_ypos = sg.Read32() / 256.0;
   m_dHeight = sg.Read32() / 256.0;
   m_dRotateAngle = sg.Read32() / 256.0;
   m_dPanAngle = sg.Read32() / 256.0;

   // read attributes and skills
   unsigned int ui=0;
   for(ui=0; ui<SDL_TABLESIZE(m_aAttributes); ui++)
      m_aAttributes[ui] = sg.Read16();

   for(ui=0; ui<SDL_TABLESIZE(m_aSkills); ui++)
      m_aSkills[ui] = sg.Read16();

   sg.EndSection();

   m_inventory.Load(sg);
   m_runebag.Load(sg);
   m_convGlobals.Load(sg);
   m_questFlags.Load(sg);
}

void Player::Save(UwBase::Savegame& sg) const
{
   sg.BeginSection(_T("player"));

   // write name
   sg.WriteString(m_cszName);

   // write position
   sg.Write32(static_cast<Uint32>(m_xpos*256.0));
   sg.Write32(static_cast<Uint32>(m_ypos*256.0));
   sg.Write32(static_cast<Uint32>(m_dHeight*256.0));
   sg.Write32(static_cast<Uint32>(m_dRotateAngle*256.0));
   sg.Write32(static_cast<Uint32>(m_dPanAngle*256.0));

   // write attributes and skills
   unsigned int ui=0;
   for(ui=0; ui<SDL_TABLESIZE(m_aAttributes); ui++)
      sg.Write16(m_aAttributes[ui]);

   for(ui=0; ui<SDL_TABLESIZE(m_aSkills); ui++)
      sg.Write16(m_aSkills[ui]);

   sg.EndSection();

   m_inventory.Save(sg);
   m_runebag.Save(sg);
   m_convGlobals.Save(sg);
   m_questFlags.Save(sg);
}
