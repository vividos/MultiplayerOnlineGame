//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file  Underworld/GameLogic/QuestFlags.cpp Quest flags
//

// includes
#include "stdafx.h"
#include "QuestFlags.hpp"
#include "Savegame.hpp"

using Underworld::QuestFlags;

// QuestFlags methods

void QuestFlags::Load(UwBase::Savegame& sg)
{
   sg.BeginSection(_T("questflags"));

   Uint16 uiMax = sg.Read16();

   m_vecQuestFlags.clear();
   m_vecQuestFlags.resize(uiMax, 0);

   for (Uint16 ui=0; ui<uiMax; ui++)
      m_vecQuestFlags[ui] = sg.Read16();

   sg.EndSection();
}

void QuestFlags::Save(UwBase::Savegame& sg) const
{
   sg.BeginSection(_T("questflags"));

   Uint16 uiMax = static_cast<Uint16>(GetFlagCount());
   sg.Write16(uiMax);

   for(Uint16 ui=0; ui<uiMax; ui++)
      sg.Write16(m_vecQuestFlags[ui]);

   sg.EndSection();
}
