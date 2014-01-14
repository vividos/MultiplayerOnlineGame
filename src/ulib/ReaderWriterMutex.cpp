//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014 Michael Fink
//
/// \file ReaderWriterMutex.cpp reader/writer mutex
//
#include "stdafx.h"
#include <ulib/ReaderWriterMutex.hpp>
#include <ulib/SystemException.hpp>

/// define this to track reader races
#define TRACK_READER_RACES

/// \brief reader writer mutex implementation
/// \details implementation that favors neither reader nor writer
/// \see http://msdn.microsoft.com/en-us/magazine/cc163405.aspx
class ReaderWriterMutex::Impl
{
public:
   /// dtor
   ~Impl() throw()
   {
      if (m_hReadyToRead != NULL)
         CloseHandle(m_hReadyToRead);

      if (m_hReadyToWrite != NULL)
         CloseHandle(m_hReadyToWrite);

      DeleteCriticalSection(&m_cs);
   }

   /// ctor
   Impl()
      :m_lWritersWaiting(0),
       m_lReadersWaiting(0),
       m_dwActiveWriterReaders(0)
#if defined(TRACK_READER_RACES)
     , m_dwReaderRacesLost(0),
       m_dwReaderWakeups(0)
#endif
   {
      InitializeCriticalSection(&m_cs);

      m_hReadyToRead = CreateEvent(NULL,
         TRUE,
         FALSE,
         NULL);

      ATLASSERT(m_hReadyToRead != NULL);

      if (m_hReadyToRead == NULL)
         throw SystemException(_T("error initializing event for ReaderWriterMutex"),
         GetLastError(), __FILE__, __LINE__);

      m_hReadyToWrite = CreateSemaphore(NULL,
         0,
         1,
         NULL);

      ATLASSERT(m_hReadyToWrite != NULL);

      if (m_hReadyToWrite == NULL)
      {
         DWORD dwLastError = GetLastError();
         CloseHandle(m_hReadyToRead);

         throw SystemException(_T("error initializing semaphore for ReaderWriterMutex"),
            dwLastError, __FILE__, __LINE__);
      }
   }

   /// acquires reader lock
   void AcquireReaderLock()
   {
      bool bNotifyReaders = false;

      EnterCriticalSection(&m_cs);

      // Block readers from acquiring the lock if
      // a writer has already acquired the lock.

      if (HIWORD(m_dwActiveWriterReaders) > 0)
      {
         ++m_lReadersWaiting;

         _ASSERTE(m_lReadersWaiting > 0);

         for (;;)
         {
            ResetEvent(m_hReadyToRead);

            LeaveCriticalSection(&m_cs);

            WaitForSingleObject(m_hReadyToRead, INFINITE);

            EnterCriticalSection(&m_cs);

#if defined(TRACK_READER_RACES)

            ++m_dwReaderWakeups;

#endif

            // A race may have ensued check to be sure 
            // that its OK to read.
            if (HIWORD(m_dwActiveWriterReaders) == 0)
            {
               break;
            }

#if defined(TRACK_READER_RACES)
            ++m_dwReaderRacesLost;
#endif
         }

         // Reader is done waiting.
         --m_lReadersWaiting;

         _ASSERTE(m_lReadersWaiting >= 0);

         // Reader can read.
         ++m_dwActiveWriterReaders;
      }
      else
      {
         // Readers can read.
         if ((++m_dwActiveWriterReaders == 1) && (m_lReadersWaiting != 0))
         {
            // Set flag to notify other waiting readers
            // outside of the critical section
            // so that they don't when the threads
            // are dispatched by the scheduler they
            // don't immediately block on the critical
            // section that this thread is holding.
            bNotifyReaders = true;
         }
      }

      _ASSERTE(HIWORD(m_dwActiveWriterReaders) == 0);

      LeaveCriticalSection(&m_cs);

      if (bNotifyReaders)
      {
         SetEvent(m_hReadyToRead);
      }
   }

   /// acquires writer lock
   void AcquireWriterLock()
   {
      EnterCriticalSection(&m_cs);

      // Are there either active readers 
      // or a active writer ?
      if (m_dwActiveWriterReaders != 0)
      {
         ++m_lWritersWaiting;

         _ASSERTE(m_lWritersWaiting > 0);

         LeaveCriticalSection(&m_cs);

         WaitForSingleObject(m_hReadyToWrite, INFINITE);

         // Upon wakeup theirs no need for the writer
         // to acquire the critical section.  It 
         // already has been transfered ownership of the
         // lock by the signaler.
      }
      else
      {
         _ASSERTE(m_dwActiveWriterReaders == 0);

         // Set that the writer owns the lock.
         m_dwActiveWriterReaders = MAKELONG(0, 1);

         LeaveCriticalSection(&m_cs);
      }
   }

   /// releases reader lock
   void ReleaseReaderLock()
   {
      EnterCriticalSection(&m_cs);

      // Assert that the lock isn't held by a writer.
      _ASSERTE(HIWORD(m_dwActiveWriterReaders) == 0);

      // Assert that the lock is held by readers.
      _ASSERTE(LOWORD(m_dwActiveWriterReaders > 0));

      // Decrement the number of active readers.
      if (--m_dwActiveWriterReaders == 0)
      {
         // No more readers active.
         ResetEvent(m_hReadyToRead);

         // if writers are waiting reset
         // the ready to read event.
         if (m_lWritersWaiting != 0)
         {
            // Decrement the number of waiting writers
            --m_lWritersWaiting;

            // Pass ownership to a writer thread.
            m_dwActiveWriterReaders = MAKELONG(0, 1);
            ReleaseSemaphore(m_hReadyToWrite, 1, NULL);
         }
      }

      LeaveCriticalSection(&m_cs);

   }

   /// releases writer lock
   void ReleaseWriterLock()
   {
      bool fNotifyWriter = false;
      bool bNotifyReaders = false;

      EnterCriticalSection(&m_cs);

      // Assert that the lock is owned by a writer.
      _ASSERTE(HIWORD(m_dwActiveWriterReaders) == 1);

      // Assert that the lock isn't owned by one or more readers
      _ASSERTE(LOWORD(m_dwActiveWriterReaders) == 0);

      if (m_lReadersWaiting > 0)
      {
         // Release the exclusive hold on the lock.
         m_dwActiveWriterReaders = 0;

         // if readers are waiting set the flag
         // that will cause the readers to be notified
         // once the critical section is released.  This
         // is done so that an awakened reader won't immediately
         // block on the critical section which is still being
         // held by this thread.
         bNotifyReaders = true;
      }
      else if (m_lWritersWaiting > 0)
      {
         // Writers waiting, decrement the number of
         // waiting writers and release the semaphore
         // which means ownership is passed to the thread
         // that has been released.
         --m_lWritersWaiting;
         fNotifyWriter = true;
      }
      else
      {
         m_dwActiveWriterReaders = 0;
      }

      LeaveCriticalSection(&m_cs);

      if (fNotifyWriter)
      {
         ReleaseSemaphore(m_hReadyToWrite, 1, NULL);
      }
      else if (bNotifyReaders)
      {
         SetEvent(m_hReadyToRead);
      }
   }

private:
   LONG m_lWritersWaiting; ///< number of writer waiting
   LONG m_lReadersWaiting; ///< number of reader waiting

   /// active writer/reader
   /// HIWORD is writer active flag;
   /// LOWORD is readers active count;
   DWORD m_dwActiveWriterReaders;

   /// handle to signal "ready to read"
   HANDLE m_hReadyToRead;

   /// handle to signal "ready to write"
   HANDLE m_hReadyToWrite;

   /// critical section to protect member variables
   CRITICAL_SECTION m_cs;

#ifdef TRACK_READER_RACES
   /// number of reader races lost
   DWORD m_dwReaderRacesLost;

   /// number of reader wakeups
   DWORD m_dwReaderWakeups;
#endif // TRACK_READER_RACES
};

ReaderWriterMutex::ReaderWriterMutex()
:m_spImpl(new Impl)
{
}

ReaderWriterMutex::~ReaderWriterMutex()
{
}

ReaderLock::ReaderLock(ReaderWriterMutex& mutex)
:m_mutex(mutex)
{
   mutex.m_spImpl->AcquireReaderLock();
}

ReaderLock::~ReaderLock()
{
   m_mutex.m_spImpl->ReleaseReaderLock();
}

WriterLock::WriterLock(ReaderWriterMutex& mutex)
:m_mutex(mutex)
{
   mutex.m_spImpl->AcquireWriterLock();
}

WriterLock::~WriterLock()
{
   m_mutex.m_spImpl->ReleaseWriterLock();
}
