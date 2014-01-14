//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/GameLogic/Runes.hpp Runebag and runeshelf
//
#pragma once

// includes
#include <vector>
#include <bitset>

namespace Underworld
{

/// \brief Rune type
/// \note in uw1 and uw2 there is no rune for Xen and Zu.
enum ERuneType
{
   runeAn=0,
   runeBet,
   runeCorp,
   runeDes,
   runeEx,
   runeFlam,
   runeGrav,
   runeHur,
   runeIn,
   runeJux,
   runeKal,
   runeLor,
   runeMani,
   runeNox,
   runeOrt,
   runePor,
   runeQuas,
   runeRel,
   runeSanct,
   runeTym,
   runeUus,
   runeVas,
   runeWis,
   runeYlem,
   runeLast = runeYlem
};

// classes

/// Runeshelf class
class Runeshelf
{
public:
   /// ctor
   Runeshelf(){}

   /// returns number of runes on shelf (max. 3 runes)
   unsigned int GetNumRunes() const { return m_vecRuneshelf.size(); }

   /// returns rune on shelf position
   ERuneType GetRune(unsigned int uiIndex) const
   {
      UaAssert(uiIndex < m_vecRuneshelf.size());
      return m_vecRuneshelf[uiIndex];
   }

   /// adds a rune to the runeshelf
   void AddRune(ERuneType rune)
   {
      if (m_vecRuneshelf.size() > 2)
         m_vecRuneshelf.erase(m_vecRuneshelf.begin());

      m_vecRuneshelf.push_back(rune);
   }

   /// resets runeshelf contents
   void Reset(){ m_vecRuneshelf.clear(); }

   // loading / saving

   /// loads runebag from savegame
   void Load(UwBase::Savegame& sg);

   /// saves runebag to a savegame
   void Save(UwBase::Savegame& sg) const;

private:
   /// runeshelf contents
   std::vector<ERuneType> m_vecRuneshelf;
};


/// Runebag class
class Runebag
{
public:
   /// ctor
   Runebag(){}

   /// checks if rune is in bag
   bool IsInBag(ERuneType rune) const { return m_runebag.test(static_cast<size_t>(rune)); }

   /// sets availability of rune in bag
   void SetRune(ERuneType rune, bool bAvail){ m_runebag.set(static_cast<size_t>(rune), bAvail); }

   /// returns runebag as bitset
   std::bitset<24>& GetBitsetRunebag(){ return m_runebag; }

   // loading / saving

   /// loads runebag from savegame
   void Load(UwBase::Savegame& sg);

   /// saves runebag to a savegame
   void Save(UwBase::Savegame& sg) const;

protected:
   /// rune bag
   std::bitset<24> m_runebag;
};

} // namespace Underworld
