//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file  Underworld/GameLogic/QuestFlags.hpp Quest flags
//
#pragma once

// includes
#include <vector>

namespace Underworld
{

/// \brief Quest flags
/// \details Quest flags are used throughout the game to store flags that are persisted
/// in savegames, e.g. to track game progress. Quest flags can be queried and
/// modified from conversations and by triggers/traps.
class QuestFlags
{
public:
   /// ctor
   QuestFlags(){}

   /// returns number of flags
   unsigned int GetFlagCount() const { return m_vecQuestFlags.size(); }

   /// returns quest flag by index
   Uint16 GetFlag(unsigned int uiIndex)
   {
      UaAssert(uiIndex < GetFlagCount());
      return m_vecQuestFlags[uiIndex];
   }

   /// sets quest flag
   void SetFlag(unsigned int uiIndex, Uint16 uiFlagValue)
   {
      UaAssert(uiIndex < GetFlagCount());
      m_vecQuestFlags[uiIndex] = uiFlagValue;
   }

   // loading / saving

   /// loads flags from savegame
   void Load(UwBase::Savegame& sg);

   /// saves flags to savegame
   void Save(UwBase::Savegame& sg) const;

private:
   /// quest flags
   std::vector<Uint16> m_vecQuestFlags;
};

} // namespace Underworld
