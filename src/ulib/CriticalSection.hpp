//
// ulib - a collection of useful classes
// Copyright (C) 2008 Michael Fink
//
/// \file CriticalSection.hpp critical section class
//
#pragma once

// includes
#include "MutexLock.hpp"

/// a non-recursive lockable object
class CriticalSection
{
public:
   /// lock type
   typedef MutexLock<CriticalSection> LockType;

   /// ctor
   CriticalSection() throw()
   {
      InitializeCriticalSection(&m_cs);
   }

   /// dtor
   ~CriticalSection() throw()
   {
      DeleteCriticalSection(&m_cs);
   }

private:
   void Lock() throw()
   {
      EnterCriticalSection(&m_cs);
   }

   void Unlock() throw()
   {
      LeaveCriticalSection(&m_cs);
   }

   // so that Lock class can call Lock() and Unlock()
   friend class MutexLock<CriticalSection>;

private:
   /// critical section object
   CRITICAL_SECTION m_cs;
};
