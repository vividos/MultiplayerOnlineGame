//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
//! \file Underworld/DebugClient.cpp Debug client
//

// includes
#include "stdafx.h"
#include "DebugClient.hpp"
#include "Game.hpp"

DebugClient::DebugClient(Game& game)
:m_game(game)
{
}

DebugClient::~DebugClient()
{
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
