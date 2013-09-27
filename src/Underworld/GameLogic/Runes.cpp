//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file  Underworld/GameLogic/Runes.hpp Runebag and runeshelf
//

// includes
#include "stdafx.h"
#include "Runes.hpp"
#include "Savegame.hpp"

using Underworld::Runebag;
using Underworld::Runeshelf;

// Runeshelf methods

void Runeshelf::Load(UwBase::Savegame& sg)
{
   sg.BeginSection(_T("runeshelf"));

   Reset();

   unsigned int count = sg.Read8();
   for (Uint8 n=0; n<count; n++)
      AddRune(static_cast<ERuneType>(sg.Read8()));

   sg.EndSection();
}

void Runeshelf::Save(UwBase::Savegame& sg) const
{
   sg.BeginSection(_T("runeshelf"));

   Uint8 count = static_cast<Uint8>(GetNumRunes() % 3);
   sg.Write8(count);

   for (unsigned int n=0; n<count; n++)
      sg.Write8(static_cast<Uint8>(GetRune(n)));

   sg.EndSection();
}


// Runebag methods

void Runebag::Load(UwBase::Savegame& sg)
{
   sg.BeginSection(_T("runebag"));

   // load runebag
   m_runebag.reset();

   Uint32 uiBagbits = sg.Read32();
   for(unsigned int i=0; i<24; i++)
      if ((uiBagbits & (1L << i)) != 0)
         m_runebag.set(i);

   sg.EndSection();
}

/// \note std::bitset::to_ulong() could be used to convert the bitset to an
/// int, but it probably isn't portable enough
void Runebag::Save(UwBase::Savegame& sg) const
{
   sg.BeginSection(_T("runebag"));

   // store runebag
   Uint32 uiBagbits = 0;
   for(unsigned int i=0; i<24; i++)
      if (m_runebag.test(i))
         uiBagbits |= 1L << i;

   sg.Write32(uiBagbits);

   sg.EndSection();
}
