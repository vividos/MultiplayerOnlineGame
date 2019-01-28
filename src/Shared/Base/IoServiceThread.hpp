//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IoServiceThread.hpp service thread for boost::asio::io_service object
//
#pragma once

// includes
#include "Base.hpp"
#include "LogCategories.hpp"
#include "Asio.hpp"
#include <ulib/Thread.hpp>
#include <thread>
#include <functional>

/// service thread for boost::asio::io_service instance
class IoServiceThread: public boost::noncopyable
{
public:
   /// ctor
   IoServiceThread(bool bDefaultWork = false, LPCTSTR pszThreadName = NULL) throw()
#ifdef _DEBUG
      :m_pszThreadName(pszThreadName)
#endif
   {
      pszThreadName;

      if (bDefaultWork)
         m_upWork.reset(new boost::asio::io_service::work(m_ioService));
   }

   /// dtor
   ~IoServiceThread() throw()
   {
      Join();
   }

   /// returns io_service object
   boost::asio::io_service& Get() throw() { return m_ioService; }

   /// runs background thread
   void Run()
   {
      m_upThread.reset(new std::thread(std::bind(&IoServiceThread::ThreadRun, this)));
   }

   /// waits for background thread
   void Join()
   {
      m_upWork.reset();

      if (m_upThread != NULL)
         m_upThread->join();

      m_upThread.reset();
   }

private:
   /// thread proc
   void ThreadRun()
   {
#ifdef _DEBUG
      if (m_pszThreadName != NULL)
         Thread::SetName(m_pszThreadName);
#endif

      LOG_INFO("IoServiceThread started background thread", Log::Threading);

      for (bool bLoop = true; bLoop;)
      {
         try
         {
            m_ioService.run();
            bLoop = false;
         }
         catch(boost::system::system_error& ex)
         {
            CString cszText;
            cszText.Format(_T("boost::system::system_error exception in io thread: %hs (%u)"),
               ex.what(), ex.code());
            LOG_ERROR(cszText, Log::Threading);
            ATLASSERT(false);
         }
         catch(Exception& ex)
         {
            CString cszText;
            cszText.Format(_T("%s(%u): exception in io thread: %s"),
               ex.SourceFile().GetString(), ex.SourceLine(), ex.Message().GetString());
            LOG_ERROR(cszText, Log::Threading);
            ATLASSERT(false);
         }
         catch(std::exception& ex)
         {
            CString cszText;
            cszText.Format(_T("std::exception in io thread: %hs"),
               ex.what());
            LOG_ERROR(cszText, Log::Threading);
            ATLASSERT(false);
         }
         catch(...)
         {
            ATLASSERT(false);
         }
      }

      LOG_INFO("IoServiceThread stopped background thread", Log::Threading);
   }

private:
   /// io service
   boost::asio::io_service m_ioService;

   /// worker thread
   std::unique_ptr<std::thread> m_upThread;

   /// default work
   std::unique_ptr<boost::asio::io_service::work> m_upWork;

#ifdef _DEBUG
   /// thread name
   LPCTSTR m_pszThreadName;
#endif
};
