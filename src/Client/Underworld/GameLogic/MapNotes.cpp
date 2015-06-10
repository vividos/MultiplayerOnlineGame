//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/GameLogic/MapNotes.cpp Map notes
//

// includes
#include "stdafx.h"
#include "MapNotes.hpp"
#include "Savegame.hpp"
#include <limits>

using Underworld::MapNotes;
using Underworld::MapNote;

// MapNotes methods

void MapNotes::Load(UwBase::Savegame& sg)
{
   sg.BeginSection("mapnotes");

   Uint16 uiEntries = sg.Read16();

   m_vecNotes.clear();
   m_vecNotes.resize(uiEntries);

   for (unsigned int i=0; i<uiEntries; i++)
   {
      MapNote& note = m_vecNotes[i];

      // read map note
      note.m_xpos = static_cast<double>(sg.Read16()) / std::numeric_limits<Uint16>::max();
      note.m_ypos = static_cast<double>(sg.Read16()) / std::numeric_limits<Uint16>::max();

      sg.ReadString(note.m_cszText);
   }
}

void MapNotes::Save(UwBase::Savegame& sg) const
{
   sg.BeginSection("mapnotes");

   Uint16 uiEntries = static_cast<Uint16>(m_vecNotes.size());
   sg.Write16(uiEntries);

   for (unsigned int i=0; i<uiEntries; i++)
   {
      const MapNote& note = m_vecNotes[i];

      sg.Write16(static_cast<Uint16>(note.m_xpos * std::numeric_limits<Uint16>::max()));
      sg.Write16(static_cast<Uint16>(note.m_ypos * std::numeric_limits<Uint16>::max()));
      sg.WriteString(note.m_cszText);
   }

   sg.EndSection();
}
