//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ThreadPool.hpp Thread pool class
//
#pragma once

// includes
#pragma warning(disable: 4913) // user defined binary operator ',' exists but no overload could convert all operands, default built-in binary operator ',' used
#include <boost/thread.hpp>
#pragma warning(default: 4913)

/// \brief Thread pool
/// at the moment only one thread is used
class ThreadPool
{
public:
   /// work function
   typedef boost::function<void()> T_fnWork;

   /// ctor
   ThreadPool()
      :m_scpWork(new boost::asio::io_service::work(m_ioService))
   {
      m_scpQueueThread.reset(
         new boost::thread(
            boost::bind(&boost::asio::io_service::run, &m_ioService)));
   }

   /// dtor; joins all worker threads
   ~ThreadPool()
   {
      m_scpWork.reset();
      if (m_scpQueueThread != NULL)
         m_scpQueueThread->join();
   }

   /// queues worker function
   void QueueWork(T_fnWork fnWork)
   {
      m_ioService.post(fnWork);
   }

   /// returns io service used
   boost::asio::io_service& GetIoService() throw() { return m_ioService; }

private:
   /// io service for queue
   boost::asio::io_service m_ioService;

   /// work item
   boost::scoped_ptr<boost::asio::io_service::work> m_scpWork;

   /// worker thread
   boost::scoped_ptr<boost::thread> m_scpQueueThread;
};
