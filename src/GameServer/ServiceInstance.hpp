//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ServiceInstance.hpp Win32 Service instance
//
#pragma once

// includes
#include <ulib/Event.hpp>
#include <thread>
#include <functional>

namespace Win32
{

/// \brief service instance
/// \details class to instanciate a service; used internally by ServiceRunner
/// to run a service. The service class only has to provide two functions,
/// Start() and Stop() that will be called by service instance to start and
/// stop the server, respectively. Note that when Start() returns, the service
/// stops, and note that calls to Stop() necessarily come from another thread.
/// \tparam TService service class
template <typename TService>
class ServiceInstance
{
public:
   /// our class type
   typedef ServiceInstance<TService> ThisType;

   /// ctor
   ServiceInstance(LPCTSTR pszLogCategory)
      :m_evtStopService(true, false), // manual-reset event
       m_evtStarted(true, false), // manual-reset event
       m_hStatus(0),
       m_cszLogCategory(pszLogCategory)
   {
   }

private:
   /// \brief service handler function
   /// \details called by SCM in separate thread to notify service about
   /// control changes.
   static DWORD WINAPI ServiceHandlerEx(
      DWORD dwControl,
      DWORD /*dwEventType*/,
      LPVOID /*lpEventData*/,
      LPVOID lpContext)
   {
      ThisType* pService =
         reinterpret_cast<ThisType*>(lpContext);

      DWORD dwError = NO_ERROR;
      switch(dwControl)
      {
      case SERVICE_CONTROL_INTERROGATE:
         break;

      case SERVICE_CONTROL_STOP:
      case SERVICE_CONTROL_SHUTDOWN:
         try
         {
            pService->m_evtStopService.Set();
         }
         catch(SystemException& ex)
         {
            // event was abandoned earlier
            dwError = ex.Win32Error();
         }
         break;
      }

      return dwError;
   }

   /// \brief service main function
   /// \details called by SCM on a separate thread to run the service
   /// specified in ServiceRunner::Add() call. Calls Main().
   /// \param dwArgc number of arguments; at least 1 always
   /// \param lpszArgv argument array; argument 0 contains the service name
   static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
   {
      dwArgc;
      ATLTRACE(_T("ServiceMain\n"));

      ATLASSERT(dwArgc > 0);
      LPCTSTR pszServiceName = lpszArgv[0];

      ATLTRACE(_T("service name: %s\n"), pszServiceName);

      Win32::ServiceInfo si = TService::GetServiceInfo();

      ServiceInstance<TService> serviceInstance(si.LogCategory());
      serviceInstance.Main(pszServiceName);
   }

   /// \brief main function for service
   /// \details called from ServiceMain
   /// \param pszServiceName service name
   void Main(LPCTSTR pszServiceName)
   {
      ATLTRACE(_T("RegisterServiceCtrlHandlerEx\n"));

      m_hStatus = ::RegisterServiceCtrlHandlerEx(
         pszServiceName,
         &ThisType::ServiceHandlerEx,
         this);

      // signal running, but don't accept controls
      UpdateStatus(SERVICE_RUNNING, 0);

      // start thread
      std::thread t(std::bind(&ThisType::ServiceThread, this));

      // wait for thread to start
      m_evtStarted.Wait(INFINITE);

      // allow controls
      UpdateStatus(SERVICE_RUNNING, SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP);

      // wait for shutdown
      for (;;)
      {
         if (m_evtStopService.Wait(INFINITE))
         {
            UpdateStatus(SERVICE_STOP_PENDING);

            // stop service thread
            m_spService->Stop();
            t.join();

            UpdateStatus(SERVICE_STOPPED);
            break;
         }
      }
   }

   /// \brief service thread
   /// \details started by Main() to run service's Run() function. The service
   /// object is created here finally.
   void ServiceThread()
   {
      ATLTRACE(_T("ServiceThread\n"));

      bool bCtorFailed = false;
      try
      {
         m_spService.reset(new TService);
      }
      catch(Exception& ex)
      {
         LOG_FATAL(_T("ServiceThread(): exception while creating service object: ") + ex.Message(),
            m_cszLogCategory);
         bCtorFailed = true;
      }
      catch(...)
      {
         LOG_FATAL(_T("ServiceThread(): unknown exception while creating service object"),
            m_cszLogCategory);
         bCtorFailed = true;
      }

      // event must be set in any case
      m_evtStarted.Set();

      if (bCtorFailed)
         return;

      try
      {
         m_spService->Run();
      }
      catch(Exception& ex)
      {
         LOG_FATAL(_T("ServiceThread(): exception while running service: ") + ex.Message(),
            m_cszLogCategory);
      }
      catch(...)
      {
         LOG_FATAL(_T("ServiceThread(): unknown exception while running service"),
            m_cszLogCategory);
      }

      ATLTRACE(_T("ServiceThread ended\n"));

      // stop service
      m_evtStopService.Set();
   }

   /// \brief updates service status
   /// \details notifies SCM about current state and accepted control codes
   void UpdateStatus(DWORD dwState, DWORD dwControlsAccepted = 0)
   {
      SERVICE_STATUS serviceStatus = {0};
      serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
      serviceStatus.dwCurrentState = dwState;
      serviceStatus.dwControlsAccepted = dwControlsAccepted;
      serviceStatus.dwWin32ExitCode = NO_ERROR;

      ::SetServiceStatus(m_hStatus, &serviceStatus);
   }

private:
   friend class ServiceRunner;

   /// service table entry for this service; created by IMPLEMENT_SERVICE
   static SERVICE_TABLE_ENTRY m_serviceTableEntry;

   /// service class
   std::shared_ptr<TService> m_spService;

   /// status handle; doesn't need to be closed
   SERVICE_STATUS_HANDLE m_hStatus;

   /// event that gets signaled when service thread has started
   Event m_evtStarted;

   /// event that gets signaled when service thread has stopped
   Event m_evtStopService;

   /// log category
   CString m_cszLogCategory;
};

} // namespace Win32

/// define to implement service table entry
#define IMPLEMENT_SERVICE(serviceName, serviceClass) \
   SERVICE_TABLE_ENTRY Win32::ServiceInstance<serviceClass>::m_serviceTableEntry = \
   { serviceName, &Win32::ServiceInstance<serviceClass>::ServiceMain };
