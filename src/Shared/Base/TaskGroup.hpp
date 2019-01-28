//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TaskGroup.hpp Task group
//
#pragma once

// includes
#include "Asio.hpp"
#include <ulib/thread/LightweightMutex.hpp>
#include <ulib/thread/Event.hpp>
#include <atomic>
#include <deque>
#include <functional>

/// \brief task group
/// \details groups together tasks to run
class TaskGroup
{
public:
   /// task type
   typedef std::function<void()> T_fnTask;

   /// ctor
   TaskGroup(boost::asio::io_service& ioService)
      :m_ioService(ioService)
   {
   }

   /// cancels all tasks not yet running
   void Cancel()
   {
      // note: this assumes that io_service only runs on one thread
      {
         MutexLock<LightweightMutex> lock(m_mtxTaskList);
         if (m_deqTaskList.empty())
            return;

         m_bIsCanceled = true;

         m_deqTaskList.clear();
      }

      // wait for all tasks to be finished
      ManualResetEvent evtEmptyQueue(false);

      m_ioService.post(std::bind(&ManualResetEvent::Set, &evtEmptyQueue));

      evtEmptyQueue.Wait();
   }

   /// clears all tasks not yet running
   void Clear()
   {
      MutexLock<LightweightMutex> lock(m_mtxTaskList);
      if (m_deqTaskList.empty())
         return;

      m_deqTaskList.clear();
   }

   /// adds task to run
   void Add(T_fnTask fnTask)
   {
      if (m_bIsCanceled)
         return;

      MutexLock<LightweightMutex> lock(m_mtxTaskList);

      m_deqTaskList.push_back(fnTask);

      m_ioService.post(std::bind(&TaskGroup::RunOneTask, this));
   }

   /// sets handler for "task queue empty" event; handler is reset after calling it once
   void SetTaskQueueEmptyHandler(std::function<void()> fnOnTaskQueueEmpty)
   {
      MutexLock<LightweightMutex> lock(m_mtxTaskList);
      m_fnOnTaskQueueEmpty = fnOnTaskQueueEmpty;
   }

private:
   /// runs one task
   void RunOneTask()
   {
      T_fnTask fnTask;

      {
         MutexLock<LightweightMutex> lock(m_mtxTaskList);
         if (m_deqTaskList.empty())
            return;

         fnTask = m_deqTaskList.front();
         m_deqTaskList.pop_front();
      }

//#ifdef _DEBUG
//      TraceOutputStopwatch<HighResolutionTimer> stopwatch(_T("TaskGroup"));
//#endif

      fnTask();

      {
         MutexLock<LightweightMutex> lock(m_mtxTaskList);
         if (m_deqTaskList.empty() && m_fnOnTaskQueueEmpty != nullptr)
         {
            m_fnOnTaskQueueEmpty();

            m_fnOnTaskQueueEmpty = nullptr;
         }
      }
   }

private:
   /// task list
   std::deque<T_fnTask> m_deqTaskList;

   /// mutex to protect task list
   LightweightMutex m_mtxTaskList;

   /// indicates if task group is in a canceled state
   std::atomic<bool> m_bIsCanceled;

   /// io service to run tasks on
   boost::asio::io_service& m_ioService;

   /// handler for task queue empty event
   std::function<void()> m_fnOnTaskQueueEmpty;
};
