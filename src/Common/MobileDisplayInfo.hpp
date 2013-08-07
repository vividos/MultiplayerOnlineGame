//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file MobileDisplayInfo.hpp Mobile display info
//
#pragma once

// includes
#include "Common.hpp"

/// display info for Mobile objects
class COMMON_DECLSPEC MobileDisplayInfo
{
public:
   /// ctor
   MobileDisplayInfo(WORD wInfo);

   /// checks if all values are valid
   bool IsValid() const throw();

   // 0-3: 4
   unsigned int BaseFigure() const throw();
   void BaseFigure(unsigned int uiBaseFigure) throw();

   // 0-2: 3
   unsigned int SkinColor() const throw();
   void SkinColor(unsigned int uiSkinColor) throw();

   // 0-7: 8
   unsigned int FaceStyle() const throw();
   void FaceStyle(unsigned int uiStyle) throw();

   // 0-4: 5
   unsigned int HairColor() const throw();
   void HairColor(unsigned int uiColor) throw();

   // 0-4: 5
   unsigned int PilosityHairStyle() const throw();
   void PilosityHairStyle(unsigned int uiStyle) throw();

   // 0-11: 12
   unsigned int PilosityBrowStyle() const throw();
   void PilosityBrowStyle(unsigned int uiStyle) throw();

   /// returns info word
   WORD Info() const throw() { return m_wInfo; }

private:
   /// display info word
   WORD m_wInfo;
};
