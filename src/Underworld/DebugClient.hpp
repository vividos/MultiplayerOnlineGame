//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
//! \file Underworld/DebugClient.hpp Debug client
//

// includes
#include <ulib/RecursiveMutex.hpp>

// forward references
struct Game;

namespace Underworld
{
   class World;
}

/// debug client
class DebugClient
{
public:
   /// ctor
   DebugClient(Game& game);
   /// dtor
   ~DebugClient();

   /// returns a lock to the game world; needed before accessing world
   RecursiveMutex::LockType GetLock();

   /// returns world
   Underworld::World& GetWorld();

private:
   /// the game object
   Game& m_game;

   /// the big debug lock
   RecursiveMutex m_debugLock;
};
