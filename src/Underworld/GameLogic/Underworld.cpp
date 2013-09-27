//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/GameLogic/Underworld.cpp Underworld object
//

// includes
#include "stdafx.h"
#include "Underworld.hpp"
#include "Savegame.hpp"

// Underworld methods

void Underworld::World::Load(UwBase::Savegame& sg)
{
   if (sg.GetVersion()<3)
   {
      UaTrace("cannot load savegames prior version 3!\n");
      return;
   }

   m_levelList.Load(sg);
   m_player.Load(sg);
}

void Underworld::World::Save(UwBase::Savegame& sg) const
{
   m_levelList.Save(sg);
   m_player.Save(sg);
}
