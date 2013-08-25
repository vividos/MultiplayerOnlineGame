//
// ulib - a collection of useful classes
// Copyright (C) 2008-2013 Michael Fink
//
/// \file RecursiveMutex.hpp recursive mutex class
//
#pragma once

// includes
#include "MutexLock.hpp"
#include <ulib/SystemException.hpp>
#include <boost/shared_ptr.hpp>

/// a recursive lockable synchronisation object
class RecursiveMutex
{
public:
   /// lock type
   typedef MutexLock<RecursiveMutex> LockType;

   /// ctor
   RecursiveMutex()
   {
      HANDLE hMutex = ::CreateMutex(NULL, FALSE, NULL);
      if (hMutex == NULL)
         throw SystemException(_T("failed to create mutex"), GetLastError(), __FILE__, __LINE__);

#ifndef _DEBUG
      m_spMutex.reset(hMutex, ::CloseHandle);
#else
      m_spMutex.reset(hMutex, &DestroyMutex);
#endif
   }

private:
#ifdef _DEBUG
   static void DestroyMutex(void* p)
   {
      ::CloseHandle(p);
   }
#endif

   /// locks object
   void Lock()
   {
      //ATLTRACE(_T("%08x: Mutex::Lock() trying to acquire lock at handle=%08x\n"),
      //   GetCurrentThreadId(), m_spMutex.get());

      DWORD dwRet = ::WaitForSingleObject(m_spMutex.get(), INFINITE);
      if (dwRet == WAIT_OBJECT_0)
      {
         //ATLTRACE(_T("%08x: Mutex::Lock() acquired lock at handle=%08x\n"),
         //   GetCurrentThreadId(), m_spMutex.get());
         return;
      }

      if (dwRet == WAIT_ABANDONED)
         throw SystemException(_T("try locking abandoned mutex"), dwRet, __FILE__, __LINE__);

      // unknown failure
      throw SystemException(_T("failed to lock recursive mutex"), GetLastError(), __FILE__, __LINE__);
   }

   /// locks object
   void Unlock()
   {
      BOOL bRet = ::ReleaseMutex(m_spMutex.get());
      if (bRet == FALSE)
         ATLTRACE(_T("RecursiveMutex::Unlock() error: didn't own mutex!\n"));

      //ATLTRACE(_T("%08x: Mutex::Unlock() released lock at handle=%08x\n"),
      //   GetCurrentThreadId(), m_spMutex.get());
   }

   // so that Lock class can call Lock() and Unlock()
   friend MutexLock<RecursiveMutex>;
   friend MutexUnLocker<RecursiveMutex>;

private:
   /// mutex handle
   std::shared_ptr<void> m_spMutex;
};
