//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/DebugClient.cpp Debug client
//

// includes
#include "stdafx.h"
#include "DebugClient.hpp"
#include "Game.hpp"

DebugClient::DebugClient(Game& game)
:m_game(game),
 m_bIsRunning(false)
{
}

DebugClient::~DebugClient()
{
}

void DebugClient::RunDebugger()
{
   if (m_bIsRunning)
      return;

   m_bIsRunning = true;

   m_upDebugThread.reset(
      new std::thread([&](){
      HMODULE hMod = LoadLibrary(_T("Debugger.dll"));

      typedef void(*T_fn)(DebugClient*);
      T_fn fn = (T_fn)GetProcAddress(hMod, "Debug");

      if (fn != nullptr)
         fn(this);

      FreeLibrary(hMod);
   }));
}

RecursiveMutex::LockType DebugClient::GetLock()
{
   return RecursiveMutex::LockType(m_debugLock);
}

Underworld::World& DebugClient::GetWorld()
{
   // TODO check if debug lock is held

   return m_game.m_world;
}
