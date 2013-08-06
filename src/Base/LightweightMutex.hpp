//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file LightweightMutex.hpp Lightweight mutex class
//
#pragma once

// includes

/// \brief lightweight, non-recursive mutex
/// \details a mutex that is optimized for non-contention cases
class LightweightMutex
{
public:
   /// ctor
   LightweightMutex()
   {
      InitializeCriticalSection(&m_cs);
   }

   /// dtor
   ~LightweightMutex()
   {
      DeleteCriticalSection(&m_cs);
   }

private:
   /// locks object
   void Lock() throw()
   {
      EnterCriticalSection(&m_cs);
   }

   /// unlocks object
   void Unlock() throw()
   {
      LeaveCriticalSection(&m_cs);
   }

   // so that Lock class can call Lock() and Unlock()
   friend MutexLock<LightweightMutex>;
   friend MutexUnLocker<LightweightMutex>;

private:
   /// critical section object
   CRITICAL_SECTION m_cs;
};
