//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file IoServiceThread.hpp service thread for boost::asio::io_service object
//
#pragma once

// includes
#include "Base.hpp"
#include "LogCategories.hpp"
#include "Asio.hpp"
#include <ulib/Thread.hpp>
#pragma warning(disable: 4913) // user defined binary operator ',' exists but no overload could convert all operands, default built-in binary operator ',' used
#include <boost/thread.hpp>
#pragma warning(default: 4913)
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
         m_spWork.reset(new boost::asio::io_service::work(m_ioService));
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
      m_spThread.reset(new boost::thread(std::bind(&IoServiceThread::ThreadRun, this)));
   }

   /// waits for background thread
   void Join()
   {
      m_spWork.reset();

      if (m_spThread != NULL)
         m_spThread->join();

      m_spThread.reset();
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
               ex.SourceFile(), ex.SourceLine(), ex.Message());
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
   boost::scoped_ptr<boost::thread> m_spThread;

   /// default work
   boost::scoped_ptr<boost::asio::io_service::work> m_spWork;

#ifdef _DEBUG
   /// thread name
   LPCTSTR m_pszThreadName;
#endif
};
