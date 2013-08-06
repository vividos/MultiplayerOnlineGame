//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Lockable.hpp Base class for all objects that may be locked
//
#pragma once

// includes
#include "LightweightMutex.hpp"

/// \brief Simple lockable class using a recursive mutex
/// Use this class as base class for all classes that need to be lockable.
class Lockable
{
public:
   /// lock class type
   typedef MutexLock<LightweightMutex> LockType;

   /// locks object, returning a lock class; when it goes out of scope, the object is unlocked
   LockType Lock(){ return LockType(m_mtx); }

private:
   /// mutex
   LightweightMutex m_mtx;
};
