//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2011-2013 Michael Fink
//
//! \file ServiceControlManager.cpp Win32 Service Control Manager (SCM) access
//

// includes
#include "StdAfx.h"
#include "ServiceControlManager.hpp"
#include "ServiceInfo.hpp"
#include <ulib/SystemException.hpp>

using Win32::ServiceControlManager;
using Win32::Service;

ServiceControlManager::ServiceControlManager()
{
   SC_HANDLE hSCM = ::OpenSCManager(
      NULL, // local machine
      NULL, // or SERVICES_ACTIVE_DATABASE
      SC_MANAGER_ALL_ACCESS);

   if (hSCM == NULL)
      throw SystemException(_T("OpenSCManager"), GetLastError(), __FILE__, __LINE__);

   m_spSCM.reset(hSCM, ::CloseServiceHandle);
}

Service ServiceControlManager::RegisterService(const Win32::ServiceInfo& service)
{
   SC_HANDLE hService = ::CreateService(
      m_spSCM.get(),
      service.ServiceName(),
      service.DisplayName(),
      SC_MANAGER_CREATE_SERVICE,
      SERVICE_WIN32_OWN_PROCESS, // or SERVICE_WIN32_SHARE_PROCESS? and SERVICE_INTERACTIVE_PROCESS?
      SERVICE_AUTO_START, // or SERVICE_DEMAND_START? or SERVICE_DISABLED?
      SERVICE_ERROR_NORMAL,
      service.CommandLine(),
      NULL, // no group
      NULL, // no tag id
      NULL, // no dependencies
      NULL, // LocalSystem account, or "NT AUTHORITY\LocalService"? or "NT AUTHORITY\NetworkService"?
      NULL // password
   );

   if (hService == NULL)
      throw SystemException(_T("CreateService"), GetLastError(), __FILE__, __LINE__);

   ServiceControlHandle spService(hService, ::CloseServiceHandle);

   // set description
   SERVICE_DESCRIPTION serviceDescription = {0};
   serviceDescription.lpDescription = const_cast<LPTSTR>(service.Description());

   BOOL bRet = ::ChangeServiceConfig2(
      spService.get(),
      SERVICE_CONFIG_DESCRIPTION,
      &serviceDescription);

   if (FALSE == bRet)
      throw SystemException(_T("ChangeServiceConfig2"), GetLastError(), __FILE__, __LINE__);

   return Service(spService);
}

void ServiceControlManager::UnregisterService(LPCTSTR pszServiceName)
{
   SC_HANDLE hService = ::OpenService(
      m_spSCM.get(),
      pszServiceName,
      DELETE);

   if (hService == NULL)
      throw SystemException(_T("OpenService"), GetLastError(), __FILE__, __LINE__);

   ServiceControlHandle spService(hService, ::CloseServiceHandle);

   BOOL bRet = ::DeleteService(hService);
   if (FALSE == bRet)
      throw SystemException(_T("DeleteService"), GetLastError(), __FILE__, __LINE__);
}

Service ServiceControlManager::OpenService(LPCTSTR pszServiceName)
{
   SC_HANDLE hService = ::OpenService(
      m_spSCM.get(),
      pszServiceName,
      SERVICE_START | SERVICE_STOP /*SERVICE_ALL_ACCESS*/);

   if (hService == NULL)
      throw SystemException(_T("OpenService"), GetLastError(), __FILE__, __LINE__);

   ServiceControlHandle spService(hService, ::CloseServiceHandle);

   return Service(spService);
}

void Service::Start()
{
   BOOL bRet = ::StartService(m_spService.get(),
      0, // num service args
      NULL); // service args array

   if (FALSE == bRet)
      throw SystemException(_T("StartService"), GetLastError(), __FILE__, __LINE__);
}

void Service::Stop()
{
   SERVICE_STATUS serviceStatus = {0};

   BOOL bRet = ::ControlService(
      m_spService.get(),
      SERVICE_CONTROL_STOP,
      &serviceStatus);

   if (FALSE == bRet)
      throw SystemException(_T("ControlService(STOP)"), GetLastError(), __FILE__, __LINE__);
}

void Service::Pause()
{
   SERVICE_STATUS serviceStatus = {0};

   BOOL bRet = ::ControlService(
      m_spService.get(),
      SERVICE_CONTROL_PAUSE,
      &serviceStatus);

   if (FALSE == bRet)
      throw SystemException(_T("ControlService(PAUSE)"), GetLastError(), __FILE__, __LINE__);
}

void Service::Restart()
{
   try
   {
      Stop();
   }
   catch(...)
   {
   }

   Start();
}
