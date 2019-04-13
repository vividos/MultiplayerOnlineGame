//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MobileDisplayInfo.cpp Mobile display info
//

// includes
#include "StdAfx.h"
#include "MobileDisplayInfo.hpp"

MobileDisplayInfo::MobileDisplayInfo(WORD wInfo)
:m_wInfo(wInfo)
{
}

/// returns bits from WORD
template <unsigned int uiStartBit, unsigned int uiCount>
unsigned int GetBits(WORD wValue)
{
   static_assert(uiStartBit + uiCount <= sizeof(WORD)*8, "start bit and count must be less that 16 bits");

   wValue >>= uiStartBit;
   WORD wMask = (1 << uiCount) - 1;
   return wValue & wMask;
}

/// sets bits in WORD
template <unsigned int uiStartBit, unsigned int uiCount>
void SetBits(WORD& wValue, unsigned int uiNewValue)
{
   static_assert(uiStartBit + uiCount <= sizeof(WORD)*8, "start bit and count must be less that 16 bits");

   WORD wMask = (1 << uiCount) - 1;
   ATLASSERT(uiNewValue < unsigned(wMask));

   wValue &= ~wMask;
   wValue |= WORD(uiNewValue << uiStartBit);
}

unsigned int MobileDisplayInfo::BaseFigure() const { return GetBits<0,2>(m_wInfo); }
unsigned int MobileDisplayInfo::SkinColor() const { return GetBits<2,2>(m_wInfo); }
unsigned int MobileDisplayInfo::HairColor() const { return GetBits<4,3>(m_wInfo); }
unsigned int MobileDisplayInfo::FaceStyle() const { return GetBits<7,3>(m_wInfo); }

void MobileDisplayInfo::BaseFigure(unsigned int ui) { return SetBits<0,2>(m_wInfo, ui); }
void MobileDisplayInfo::SkinColor(unsigned int ui) { return SetBits<2,2>(m_wInfo, ui); }
void MobileDisplayInfo::HairColor(unsigned int ui) { return SetBits<4,3>(m_wInfo, ui); }
void MobileDisplayInfo::FaceStyle(unsigned int ui) { return SetBits<7,3>(m_wInfo, ui); }

unsigned int MobileDisplayInfo::PilosityHairStyle() const
{
   return GetBits<10,6>(m_wInfo) % 5;
}

void MobileDisplayInfo::PilosityHairStyle(unsigned int uiStyle)
{
   return SetBits<10,6>(m_wInfo, PilosityBrowStyle() * 5 + uiStyle);
}

unsigned int MobileDisplayInfo::PilosityBrowStyle() const
{
   return GetBits<10,6>(m_wInfo) / 5;
}

void MobileDisplayInfo::PilosityBrowStyle(unsigned int uiStyle)
{
   return SetBits<10,6>(m_wInfo, uiStyle * 5 + PilosityHairStyle());
}

bool MobileDisplayInfo::IsValid() const
{
   // the commented out tests are never true due to the bit range in storage
//   if (BaseFigure() > 3) return false;
   if (SkinColor() > 2) return false;
//   if (FaceStyle() > 7) return false;
   if (HairColor() > 4) return false;
//   if (PilosityHairStyle() > 4) return false;
   if (PilosityBrowStyle() > 11) return false;

   return true;
}
