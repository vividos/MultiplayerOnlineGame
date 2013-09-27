//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file  Underworld/GameLogic/ConvGlobals.cpp Conversation globals
//

// includes
#include "stdafx.h"
#include "ConvGlobals.hpp"
#include "Savegame.hpp"

using Underworld::ConvGlobals;

// ConvGlobals methods

void ConvGlobals::Load(UwBase::Savegame& sg)
{
   sg.BeginSection(_T("conv.globals"));

   // get number of slots
   Uint16 uiMaxSlots = sg.Read16();

   m_vecAllGlobals.clear();
   m_vecAllGlobals.reserve(uiMaxSlots);

   for (Uint16 uiSlot=0; uiSlot<uiMaxSlots; uiSlot++)
   {
      // get number of slot globals
      Uint16 uiMaxGlobals = sg.Read16();

      ConvSlotGlobals vecSlotGlobals;
      vecSlotGlobals.reserve(uiMaxGlobals);

      // get all slot globals
      for(Uint16 ui=0; ui<uiMaxGlobals; ui++)
         vecSlotGlobals.push_back(sg.Read16());

      m_vecAllGlobals.push_back(vecSlotGlobals);
   }

   sg.EndSection();
}

void ConvGlobals::Save(UwBase::Savegame& sg) const
{
   sg.BeginSection(_T("conv.globals"));

   // write number of conv slots
   Uint16 uiMaxSlots = static_cast<Uint16>(m_vecAllGlobals.size());
   sg.Write16(uiMaxSlots);

   // for each slot ...
   for (unsigned int uiSlot=0; uiSlot<uiMaxSlots; uiSlot++)
   {
      const ConvSlotGlobals& vecSlotGlobals = m_vecAllGlobals[uiSlot];

      // write number of slot globals
      Uint16 uiMaxGlobals = static_cast<Uint16>(vecSlotGlobals.size());
      sg.Write16(uiMaxGlobals);

      // write all globals
      for(unsigned int ui=0; ui<uiMaxGlobals; ui++)
         sg.Write16(vecSlotGlobals[ui]);
   }

   sg.EndSection();
}
