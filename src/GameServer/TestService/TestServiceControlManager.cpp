//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestServiceControlManager.cpp Unit tests for class ServiceControlManager
//

// includes
#include "stdafx.h"
#include "TestServiceControlManager.hpp"
#include "ServiceControlManager.hpp"
#include "TestService.hpp"

using UnitTest::TestServiceControlManager;

bool TestServiceControlManager::RunTests()
{
   bool bRet = true;

   try
   {
      TestDefaultCtor();
      TestRegisterUnregister();
      TestRegisterOpenUnregister();
      TestStartStop();
      TestStartRestartStop();
      TestStartUnregister();
      TestStartRegister();
      TestOpenRestart();
      TestStopNotRunning();
   }
   catch (Exception& ex)
   {
      _tprintf(_T("Exception during unit tests: %s\n\n"), ex.Message().GetString());
      bRet = false;
   }

   CleanupUnregisterService();

   return bRet;
}

/// tests default ctor
void TestServiceControlManager::TestDefaultCtor()
{
   _tprintf(_T("Testing opening service control manager\n"));

   Win32::ServiceControlManager scm;
}

/// tests registering and unregistering service
void TestServiceControlManager::TestRegisterUnregister()
{
   _tprintf(_T("Testing registering and unregistering service\n"));

   Win32::ServiceControlManager scm;

   Win32::ServiceInfo si = TestService::GetServiceInfo();
   scm.RegisterService(si);
   scm.UnregisterService(si.ServiceName());
}

/// tests opening service
void TestServiceControlManager::TestRegisterOpenUnregister()
{
   _tprintf(_T("Testing opening service\n"));

   Win32::ServiceControlManager scm;

   Win32::ServiceInfo si = TestService::GetServiceInfo();
   scm.RegisterService(si);

   Win32::Service s = scm.OpenService(si.ServiceName());

   scm.UnregisterService(si.ServiceName());
}

/// tests starting and stopping service
void TestServiceControlManager::TestStartStop()
{
   _tprintf(_T("Testing starting and stopping service\n"));

   Win32::ServiceControlManager scm;

   Win32::ServiceInfo si = TestService::GetServiceInfo();
   scm.RegisterService(si);

   Win32::Service s = scm.OpenService(si.ServiceName());
   s.Start();
   s.Stop();

   scm.UnregisterService(si.ServiceName());
}

/// tests start, restart and stop
void TestServiceControlManager::TestStartRestartStop()
{
   _tprintf(_T("Testing start, restart and stop\n"));

   Win32::ServiceControlManager scm;

   Win32::ServiceInfo si = TestService::GetServiceInfo();
   scm.RegisterService(si);

   Win32::Service s = scm.OpenService(si.ServiceName());
   s.Start();
   s.Restart();
   s.Stop();

   scm.UnregisterService(si.ServiceName());
}

/// tests starting, then unregistering service
void TestServiceControlManager::TestStartUnregister()
{
   _tprintf(_T("Testing starting, then unregistering service\n"));

   Win32::ServiceControlManager scm;

   Win32::ServiceInfo si = TestService::GetServiceInfo();
   scm.RegisterService(si);

   Win32::Service s = scm.OpenService(si.ServiceName());
   s.Start();

   scm.UnregisterService(si.ServiceName());
}

/// tests starting, then re-registering service
void TestServiceControlManager::TestStartRegister()
{
   _tprintf(_T("Testing starting, then re-registering service\n"));

   Win32::ServiceControlManager scm;

   Win32::ServiceInfo si = TestService::GetServiceInfo();
   scm.RegisterService(si);

   Win32::Service s = scm.OpenService(si.ServiceName());
   s.Start();

   scm.RegisterService(si);

   scm.UnregisterService(si.ServiceName());
}

/// tests registering, then restarting service
void TestServiceControlManager::TestOpenRestart()
{
   _tprintf(_T("Testing registering, then restarting service\n"));

   Win32::ServiceControlManager scm;

   Win32::ServiceInfo si = TestService::GetServiceInfo();
   scm.RegisterService(si);

   Win32::Service s = scm.OpenService(si.ServiceName());
   s.Restart();

   scm.UnregisterService(si.ServiceName());
}

/// tests stopping a non-running service
void TestServiceControlManager::TestStopNotRunning()
{
   _tprintf(_T("Testing stopping a non-running service\n"));

   Win32::ServiceControlManager scm;

   Win32::ServiceInfo si = TestService::GetServiceInfo();
   scm.RegisterService(si);

   Win32::Service s = scm.OpenService(si.ServiceName());
   try
   {
      s.Stop();

      // must throw exception
      throw Exception(_T("stopping a non-running service must throw exception"), __FILE__, __LINE__);
   }
   catch(...)
   {
      // ok
   }

   scm.UnregisterService(si.ServiceName());
}

void TestServiceControlManager::CleanupUnregisterService()
{
   _tprintf(_T("Cleaning up test service\n"));

   try
   {
      Win32::ServiceControlManager scm;

      Win32::ServiceInfo si = TestService::GetServiceInfo();

      scm.UnregisterService(si.ServiceName());
   }
   catch (...)
   {
   }
}
