//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/DebugClient.hpp Debug client
//

// includes
#include <ulib/RecursiveMutex.hpp>
#include <thread>
#include <atomic>

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

   /// runs the debugger
   void RunDebugger();

   /// returns a lock to the game world; needed before accessing world
   RecursiveMutex::LockType GetLock();

   /// returns game object
   Game& GetGame();

   /// returns world
   Underworld::World& GetWorld();

private:
   /// indicates if debugger already runs
   std::atomic<bool> m_bIsRunning;

   /// debugger thread
   std::unique_ptr<std::thread> m_upDebugThread;

   /// the game object
   Game& m_game;

   /// the big debug lock
   RecursiveMutex m_debugLock;
};
