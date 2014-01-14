//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/GameLogic/LevelList.cpp Level list
//

// includes
#include "stdafx.h"
#include "LevelList.hpp"
#include "Savegame.hpp"

using Underworld::LevelList;
using Underworld::Level;

// LevelList methods

void LevelList::Load(UwBase::Savegame& sg)
{
   sg.BeginSection(_T("levels"));

   size_t uiMax = sg.Read32();

   m_vecLevels.clear();
   m_vecLevels.resize(uiMax);

   for (size_t i=0; i<uiMax; i++)
   {
      std::shared_ptr<Level> spLevel(new Level);
      spLevel->Load(sg);

      m_vecLevels[i] = spLevel;
   }

   sg.EndSection();
}

void LevelList::Save(UwBase::Savegame& sg) const
{
   sg.BeginSection(_T("levels"));

   size_t uiMax = m_vecLevels.size();
   sg.Write32(static_cast<Uint32>(uiMax));

   for (size_t i=0; i<m_vecLevels.size(); i++)
      m_vecLevels[i]->Save(sg);

   sg.EndSection();
}
