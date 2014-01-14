//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/GameLogic/Underworld.hpp Underworld main object
//
#pragma once

// includes
#include "LevelList.hpp"
#include "Player.hpp"
#include "Properties.hpp"

// forward references
namespace UwBase
{
   class Savegame;
}

/// underworld objects
namespace Underworld
{

/// underworld world
class World
{
public:
   /// ctor
   World() {}

   // access to underworld components

   /// returns level list
   LevelList& GetLevelList(){ return m_levelList; }
   /// returns level list; const version
   const LevelList& GetLevelList() const { return m_levelList; }

   /// returns player
   Player& GetPlayer(){ return m_player; }
   /// returns player
   const Player& GetPlayer() const { return m_player; }

   /// returns object properties
   ObjectProperties& GetObjectProperties(){ return m_properties; }
   /// returns player
   const ObjectProperties& GetObjectProperties() const { return m_properties; }

   // loading / saving

   /// loads the underworld from a savegame
   void Load(UwBase::Savegame& sg);

   /// saves the underworld to a savegame
   void Save(UwBase::Savegame& sg) const;

private:
   /// list with all levels
   LevelList m_levelList;

   /// player
   Player m_player;

   /// object properties
   ObjectProperties m_properties;
};

} // namespace Underworld
