//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file TaskGroup.hpp Task group
//
#pragma once

// includes
#include "Asio.hpp"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <ulib/Event.hpp>
#include <ulib/Mutex.hpp>
#include "LightweightMutex.hpp"
#include <deque>

/// \brief task group
/// \details groups together tasks to run
class TaskGroup
{
public:
   typedef boost::function<void()> T_fnTask;

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

         m_deqTaskList.clear();
      }

      // wait for all tasks to be finished
      Event evtEmptyQueue(true, false); // manual-reset

      m_ioService.post(boost::bind(&Event::Set, &evtEmptyQueue));

      evtEmptyQueue.Wait();

      // clear task list again; could have been added to while waiting
      {
         MutexLock<LightweightMutex> lock(m_mtxTaskList);
         if (m_deqTaskList.empty())
            return;

         m_deqTaskList.clear();
      }
   }

   /// adds task to run
   void Add(T_fnTask fnTask)
   {
      MutexLock<LightweightMutex> lock(m_mtxTaskList);
      m_deqTaskList.push_back(fnTask);

      m_ioService.post(boost::bind(&TaskGroup::RunOneTask, this));
   }

private:
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
   std::deque<T_fnTask> m_deqTaskList;

   LightweightMutex m_mtxTaskList;

   /// io service to run tasks on
   boost::asio::io_service& m_ioService;
};
