//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MobileDisplayInfo.hpp Mobile display info
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
   bool IsValid() const;

   /// returns base figure; values 0-3 (0: muscular, 1: fat, 2: wizard, 3: female)
   unsigned int BaseFigure() const;
   /// sets base figure
   void BaseFigure(unsigned int uiBaseFigure);

   /// returns skin color; values 0-2
   unsigned int SkinColor() const;
   /// sets skin color
   void SkinColor(unsigned int uiSkinColor);

   /// returns face style; values 0-7
   unsigned int FaceStyle() const;
   /// sets face style
   void FaceStyle(unsigned int uiStyle);

   /// returns hair color; values 0-4
   unsigned int HairColor() const;
   /// sets hair color
   void HairColor(unsigned int uiColor);

   /// returns pilosity hair style; values 0-4
   unsigned int PilosityHairStyle() const;
   /// sets pilosity hair style
   void PilosityHairStyle(unsigned int uiStyle);

   /// returns pilosity brow style; vaues 0-11
   unsigned int PilosityBrowStyle() const;
   /// sets pilosity brow style
   void PilosityBrowStyle(unsigned int uiStyle);

   /// returns info word
   WORD Info() const { return m_wInfo; }

private:
   /// display info word
   WORD m_wInfo;
};
