//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SimpleServiceRunner.hpp Simple Win32 Service runner
//
#pragma once

// includes
#include "ServiceProgramOptions.hpp"
#include "AttachedConsole.hpp"
#include "ServiceInstance.hpp"
#include "ServiceRunner.hpp"
#include "ServiceControlManager.hpp"

namespace Win32
{

/// manages command line parameters and handles service running
template <typename TService>
class SimpleServiceRunner
{
public:
   /// this class type
   typedef SimpleServiceRunner<TService> ThisType;

   /// ctor
   SimpleServiceRunner() throw()
   {
   }

   /// starts service runner
   void Run(LPCTSTR pszCommandLine = ::GetCommandLine())
   {
      Win32::AttachedConsole con;

      Win32::ServiceProgramOptions spo;
      spo.RegisterHelpOption();

      // register output handler
      if (!con.IsCalledFromConsole())
         spo.RegisterOutputHandler(std::bind(&ThisType::OutputMessageBox, std::placeholders::_1));
      else
         spo.RegisterOutputHandler(std::bind(&ProgramOptions::OutputConsole, std::placeholders::_1));

      spo.Parse(pszCommandLine);

      if (spo.IsSelectedHelpOption())
         return;

      try
      {
         DoAction(spo.ServiceAction(), con.IsCalledFromConsole());
      }
      catch(Exception& ex)
      {
         ATLTRACE(_T("%s(%u): Exception: %s\n"),
            ex.SourceFile(),
            ex.SourceLine(),
            ex.Message());

         if (con.IsCalledFromConsole())
            _tprintf(_T("Exception: %s\n\n"), ex.Message());
         else
            OutputMessageBox(_T("Exception: ") + ex.Message() + _T("\n\n"));

         throw;
      }
   }

private:
   /// carries out service action
   void DoAction(ServiceProgramOptions::T_enServiceAction enServiceAction, bool bStartedFromConsole)
   {
      ServiceInfo si = TService::GetServiceInfo();

      switch(enServiceAction)
      {
      case ServiceProgramOptions::actionRun:
         {
            if (bStartedFromConsole)
            {
               _tprintf(_T("%s\n\nThis application cannot be run from the console.\n\n"), si.DisplayName());
               return;
            }

            Win32::ServiceRunner sr;
            sr.AddService<TService>();

            try
            {
               sr.Run();
            }
            catch(SystemException& ex)
            {
               if (ex.Win32Error() == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
               {
                  // not started as service
                  OutputMessageBox(_T("This application can only be run as Win32 Service."));
                  return;
               }

               throw;
            }
         }
         break;

      case ServiceProgramOptions::actionTest:
         {
            if (bStartedFromConsole)
               _tprintf(_T("%s - Testing service\n\n"), si.DisplayName());

            TService s;
            s.Run();
         }
         break;

      case ServiceProgramOptions::actionStart:
         {
            if (bStartedFromConsole)
               _tprintf(_T("%s - Starting service\n\n"), si.DisplayName());

            Win32::ServiceControlManager scm;
            Win32::Service s = scm.OpenService(si.ServiceName());
            s.Start();
         }
         break;

      case ServiceProgramOptions::actionStop:
         {
            if (bStartedFromConsole)
               _tprintf(_T("%s - Stopping service\n\n"), si.DisplayName());

            Win32::ServiceControlManager scm;
            Win32::Service s = scm.OpenService(si.ServiceName());
            s.Stop();
         }
         break;

      case ServiceProgramOptions::actionRegister:
         {
            if (bStartedFromConsole)
               _tprintf(_T("%s - Registering service\n\n"), si.DisplayName());

            Win32::ServiceControlManager scm;

            Win32::ServiceInfo si = TService::GetServiceInfo();
            Win32::Service s = scm.RegisterService(si);
         }
         break;

      case ServiceProgramOptions::actionUnregister:
         {
            if (bStartedFromConsole)
               _tprintf(_T("%s - Unregistering service\n\n"), si.DisplayName());

            Win32::ServiceControlManager scm;
            scm.UnregisterService(si.ServiceName());
         }
         break;

      default: 
         ATLASSERT(false);
      }
   }

private:
   /// output handler; uses MessageBox
   static void OutputMessageBox(const CString& cszText)
   {
      ServiceInfo si = TService::GetServiceInfo();
      ::MessageBox(GetActiveWindow(), cszText, si.DisplayName(), MB_OK);
   }
};

} // namespace Win32
