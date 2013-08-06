//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2009,2012 Michael Fink
//
/// \file TestReaderWriterMutex.cpp tests for ReaderWriterMutex class
//

// includes
#include "stdafx.h"
#include <ulib/ReaderWriterMutex.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

const int DEFAULT_READERS = 1;
const int DEFAULT_WRITERS = 1;

const int DEFAULT_ITERATIONS = 2000000;
const int DEFAULT_READER_WORK_WHILE_HOLDING_LOCK = 0;
const int DEFAULT_WRITER_WORK_WHILE_HOLDING_LOCK = 0;
const int DEFAULT_READER_WORK_INBETWEEEN_LOCK_ACQUISTIONS = 0;
const int DEFAULT_WRITER_WORK_INBETWEEEN_LOCK_ACQUISTIONS = 0;

struct ProgramArgs
{
   DWORD numReaderThreads;
   DWORD numWriterThreads;
   int numReaderIterations;
   int numWriterIterations;
   int readerWorkWhileHoldingLock;
   int writerWorkWhileHoldingLock;
   int readerWorkInBetweenLockAcquisitions;
   int writerWorkInBetweenLockAcquisitions;

   ProgramArgs()
      : numReaderThreads(DEFAULT_READERS),
        numWriterThreads(DEFAULT_WRITERS),
        numReaderIterations(DEFAULT_ITERATIONS),
        numWriterIterations(DEFAULT_ITERATIONS),
        readerWorkWhileHoldingLock(DEFAULT_READER_WORK_WHILE_HOLDING_LOCK),
        writerWorkWhileHoldingLock(DEFAULT_WRITER_WORK_WHILE_HOLDING_LOCK),
        readerWorkInBetweenLockAcquisitions(DEFAULT_READER_WORK_INBETWEEEN_LOCK_ACQUISTIONS),
        writerWorkInBetweenLockAcquisitions(DEFAULT_WRITER_WORK_INBETWEEEN_LOCK_ACQUISTIONS)
   {
   }
};

struct ThreadTimes
{
   enum
   {
      Reader = 0,
      Writer = 1
   };

   int type;
   int updatesObserved;
   LARGE_INTEGER startTime;
   LARGE_INTEGER endTime;
};

struct ThreadArgs
{
   int numReaderIterations;
   int numWriterIterations;
   int readerWorkWhileHoldingLock;
   int writerWorkWhileHoldingLock;
   int readerWorkInBetweenLockAcquisitions;
   int writerWorkInBetweenLockAcquisitions;
   int thdIndex;
   void* valueHolder;
   HANDLE hThreadWaitingEvent;
   HANDLE hThreadWakeupEvent;
   ThreadTimes* thdTimes;
};


class ValueHolder 
{
public:
   ValueHolder(ReaderWriterMutex& rwLock)
      : rwLock_(rwLock),
      value_(0),
      workAccum_(0)
   {
   }

   LONG getValue()
   {
      LONG result;

      {
         ReaderLock lock(rwLock_);

         result = value_;
      }

      return result;
   }

   LONG getValue(int fakeWork)
   {
      LONG result;

      {
         ReaderLock lock(rwLock_);

         for (int i = 0; i < fakeWork; ++i)
         {
            workAccum_ += i;
         }

         result = value_;
      }

      return result;
   }

   void setValue(LONG newValue)
   {
      WriterLock lock(rwLock_);

      value_ = newValue;
   }

   void updateValue(int discardValue, int fakeWork)
   {
      WriterLock lock(rwLock_);

      for (int i = 0; i < fakeWork; ++i)
      {
         workAccum_ += discardValue;
      }

      ++value_;
   }

private:
   ReaderWriterMutex& rwLock_;
   LONG value_;

   // Dummy used to stall when doing fake work.
   int workAccum_;
};

unsigned int __stdcall ReaderThreadFunc(void* args)
{
   ThreadArgs* thdArgs = static_cast<ThreadArgs*>(args);
   ThreadTimes* thdTimes = thdArgs->thdTimes + thdArgs->thdIndex;
   ValueHolder* valueHolder = static_cast<ValueHolder*>(thdArgs->valueHolder);
   int numIterations = thdArgs->numReaderIterations;

   thdTimes->type = ThreadTimes::Reader;
   thdTimes->updatesObserved = 0;

   SignalObjectAndWait(thdArgs->hThreadWaitingEvent, thdArgs->hThreadWakeupEvent, INFINITE, FALSE);

   QueryPerformanceCounter(&thdTimes->startTime);

   LONG v = 0;

   if ((thdArgs->readerWorkInBetweenLockAcquisitions == 0) || (thdArgs->readerWorkWhileHoldingLock == 0))
   {
      for (int i = 0; i < numIterations; ++i)
      {
         v = valueHolder->getValue();
      }
   }
   else
   {
      int workWhileHoldingLock = thdArgs->readerWorkWhileHoldingLock;
      int workBetweenAcquisitions = thdArgs->readerWorkInBetweenLockAcquisitions;
      int updatesObserved = 0;
      int oldValue = 0;
      for (int i = 0; i < numIterations; ++i)
      {
         v = valueHolder->getValue(workWhileHoldingLock);

         if (v != oldValue)
         {
            ++updatesObserved;
            oldValue = v;
         }

         for (int w = 0; w < workBetweenAcquisitions; ++w)
         {
            v = v + (w % 10);
         }
      }

      thdTimes->updatesObserved = updatesObserved;
   }

   QueryPerformanceCounter(&thdTimes->endTime);

   return v;
}

unsigned int __stdcall WriterThreadFunc(void* args)
{
   ThreadArgs* thdArgs = static_cast<ThreadArgs*>(args);
   ThreadTimes* thdTimes = thdArgs->thdTimes + thdArgs->thdIndex;
   ValueHolder* valueHolder = static_cast<ValueHolder*>(thdArgs->valueHolder);
   int numIterations = thdArgs->numWriterIterations;

   thdTimes->type = ThreadTimes::Writer;

   SignalObjectAndWait(thdArgs->hThreadWaitingEvent, thdArgs->hThreadWakeupEvent, INFINITE, FALSE);

   QueryPerformanceCounter(&thdTimes->startTime);

   if ((thdArgs->writerWorkInBetweenLockAcquisitions == 0) && (thdArgs->writerWorkWhileHoldingLock == 0))
   {
      for (int i = 0; i < numIterations; ++i)
      {
         valueHolder->setValue(i);
      }
   }
   else
   {
      int workWhileHoldingLock = thdArgs->writerWorkWhileHoldingLock;
      int workBetweenAcquisitions = thdArgs->writerWorkInBetweenLockAcquisitions;
      int newV = 0;

      for (int i = 0; i < numIterations; ++i)
      {
         valueHolder->updateValue(newV, workWhileHoldingLock);

         for (int w = 0; w < workBetweenAcquisitions; ++w)
         {
            newV = newV + (w % 10);
         }
      }
   }

   QueryPerformanceCounter(&thdTimes->endTime);

   return numIterations;
}

/// tests reader writer mutex
TEST_CLASS(TestReaderWriterMutex)
{
   /// tests TestReaderWriterMutex with reader/writer
   TEST_METHOD(TestReaderWriter)
   {
      LARGE_INTEGER perfFreq;

      // Must run on platform that supports QueryPerformanceFrequency.
      Assert::IsTrue(TRUE == QueryPerformanceFrequency(&perfFreq));

      unsigned (__stdcall * readerStartAddress) (void *);
      unsigned (__stdcall * writerStartAddress) (void *);

      ProgramArgs progArgs;

      progArgs.numReaderThreads = 1;
      progArgs.numWriterThreads = 1;
      DWORD totalThreads = progArgs.numReaderThreads + progArgs.numWriterThreads;

      ThreadArgs thdArgs;

      thdArgs.numReaderIterations = progArgs.numReaderIterations;
      thdArgs.numWriterIterations = progArgs.numWriterIterations;
      thdArgs.readerWorkWhileHoldingLock = progArgs.readerWorkWhileHoldingLock;
      thdArgs.writerWorkWhileHoldingLock = progArgs.writerWorkWhileHoldingLock;
      thdArgs.readerWorkInBetweenLockAcquisitions = progArgs.readerWorkInBetweenLockAcquisitions;
      thdArgs.writerWorkInBetweenLockAcquisitions = progArgs.writerWorkInBetweenLockAcquisitions;
      thdArgs.hThreadWaitingEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
      thdArgs.hThreadWakeupEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
      thdArgs.thdTimes = new ThreadTimes[totalThreads];

      ReaderWriterMutex rwLock;
      ValueHolder valHolder(rwLock);

      readerStartAddress = ReaderThreadFunc;
      writerStartAddress = WriterThreadFunc;
      thdArgs.valueHolder = &valHolder;

      HANDLE* threads = new HANDLE[totalThreads];

      HANDLE* nextThd = threads;

      int thdTimesIndex = 0;

      // Create the readers
      for (DWORD i = 0; i < progArgs.numReaderThreads; ++i, ++nextThd)
      {
         thdArgs.thdIndex = thdTimesIndex++;

         *nextThd = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, readerStartAddress, &thdArgs, FALSE, NULL));

         WaitForSingleObject(thdArgs.hThreadWaitingEvent, INFINITE);
      }

      // Create the writers
      for (DWORD i = 0; i < progArgs.numWriterThreads; ++i, ++nextThd)
      {
         thdArgs.thdIndex = thdTimesIndex++;

         *nextThd = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, writerStartAddress, &thdArgs, TRUE, NULL));

         WaitForSingleObject(thdArgs.hThreadWaitingEvent, INFINITE);
      }

      LARGE_INTEGER startTime;
      LARGE_INTEGER endTime;

      QueryPerformanceCounter(&startTime);

      SetEvent(thdArgs.hThreadWakeupEvent);

      WaitForMultipleObjects(totalThreads, threads, TRUE, INFINITE);

      QueryPerformanceCounter(&endTime);

      for (nextThd = threads; nextThd < (threads + totalThreads); ++nextThd)
      {
         CloseHandle(*nextThd);
      }

      delete [] threads;

      CloseHandle(thdArgs.hThreadWaitingEvent);
      CloseHandle(thdArgs.hThreadWakeupEvent);

      double timeInSecs;
      LONGLONG timeSpan;

      ATLTRACE(_T("Thread Timings:"));


      // Calculate timings.
      for (DWORD i = 0; i < totalThreads; ++i)
      {
         ThreadTimes* thdTimes = thdArgs.thdTimes + i;
         timeSpan = thdTimes->endTime.QuadPart - thdTimes->startTime.QuadPart;
         timeInSecs = static_cast<double>(timeSpan) / static_cast<double>(perfFreq.QuadPart);

         CString cszUpdatesObserved;
         if (thdTimes->type == ThreadTimes::Reader)
            cszUpdatesObserved.Format(_T(", updates observed: %i"), thdTimes->updatesObserved);

         CString cszText;
         cszText.Format(_T("\t%s %u starts @%I64u, ends @%I64u, dur. %u.%03u secs.%s"),
            (thdTimes->type == ThreadTimes::Reader) ? _T("Reader") : _T("Writer"),
            i, thdTimes->startTime.QuadPart,
            thdTimes->endTime.QuadPart,
            unsigned(timeInSecs), unsigned((timeInSecs-unsigned(timeInSecs))*1000.0),
            cszUpdatesObserved.GetString());

         ATLTRACE(cszText);
      }

      delete [] thdArgs.thdTimes;

      timeSpan = endTime.QuadPart - startTime.QuadPart;

      timeInSecs = static_cast<double>(timeSpan) / static_cast<double>(perfFreq.QuadPart);

      ATLTRACE(_T("Total execution time is %u.%03u seconds.\n"),
         unsigned(timeInSecs), unsigned((timeInSecs-unsigned(timeInSecs))*1000.0));
   }
};

} // namespace UnitTest
