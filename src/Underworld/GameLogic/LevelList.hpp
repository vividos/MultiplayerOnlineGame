//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/GameLogic/LevelList.hpp Level list
//
#pragma once

// includes
#include "Level.hpp"

namespace Underworld
{

/// list of all levels
class LevelList
{
public:
   /// ctor
   LevelList(){}

   /// returns number of levels in list
   unsigned int GetNumLevels() const { return m_vecLevels.size(); }

   /// returns level
   Level& GetLevel(unsigned int uiLevel)
   {
      UaAssert(uiLevel < GetNumLevels());
      return *m_vecLevels[uiLevel];
   }

   /// returns level
   const Level& GetLevel(unsigned int uiLevel) const
   {
      UaAssert(uiLevel < GetNumLevels());
      return *m_vecLevels[uiLevel];
   }

   // loading/saving

   /// loads levelmaps from savegame
   void Load(UwBase::Savegame& sg);

   /// saves levelmaps to savegame
   void Save(UwBase::Savegame& sg) const;

   /// returns all levelmaps in a vector
   std::vector<std::shared_ptr<Level>>& GetVectorLevels(){ return m_vecLevels; }

private:
   /// all underworld levels
   std::vector<std::shared_ptr<Level>> m_vecLevels;
};

} // namespace Underworld
