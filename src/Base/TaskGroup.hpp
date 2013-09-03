//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file TaskGroup.hpp Task group
//
#pragma once

// includes
#include "Asio.hpp"
#include "LightweightMutex.hpp"
#include <ulib/Event.hpp>
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
      Event evtEmptyQueue(true, false); // manual-reset

      m_ioService.post(std::bind(&Event::Set, &evtEmptyQueue));

      evtEmptyQueue.Wait();
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
};
