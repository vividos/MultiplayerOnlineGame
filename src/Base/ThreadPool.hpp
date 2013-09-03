//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ThreadPool.hpp Thread pool class
//
#pragma once

// includes
#include <thread>
#include <functional>
#include "Asio.hpp"

/// \brief Thread pool
/// at the moment only one thread is used
class ThreadPool
{
public:
   /// work function
   typedef std::function<void()> T_fnWork;

   /// ctor
   ThreadPool()
      :m_upWork(new boost::asio::io_service::work(m_ioService))
   {
      m_upQueueThread.reset(
         new std::thread(
            std::bind(&boost::asio::io_service::run, &m_ioService)));
   }

   /// dtor; joins all worker threads
   ~ThreadPool()
   {
      m_upWork.reset();
      if (m_upQueueThread != NULL)
         m_upQueueThread->join();
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
   std::unique_ptr<boost::asio::io_service::work> m_upWork;

   /// worker thread
   std::unique_ptr<std::thread> m_upQueueThread;
};
