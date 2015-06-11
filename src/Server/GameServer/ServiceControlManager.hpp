//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ServiceControlManager.hpp Win32 Service Control Manager (SCM) access
//
#pragma once

/// \brief Win32 classes
namespace Win32
{
// forward references
class ServiceInfo;

/// service control handle type
typedef std::shared_ptr<SC_HANDLE__> ServiceControlHandle;

/// \brief service access
/// \details instances of this class are returned by ServiceControlManager.
class Service
{
public:
   /// starts service
   void Start();

   /// stops service
   void Stop();

   /// pauses service
   void Pause();

   /// restarts service
   void Restart();

private:
   /// ctor
   Service(ServiceControlHandle spService)
      :m_spService(spService)
   {
   }

private:
   friend class ServiceControlManager;

   /// service handle
   ServiceControlHandle m_spService;
};

/// \brief service control manager
/// \details administrator access may be needed to access SCM
class ServiceControlManager
{
public:
   /// ctor
   ServiceControlManager();

   /// registers service
   Service RegisterService(const ServiceInfo& service);

   /// unregisters service by service name
   void UnregisterService(LPCTSTR pszServiceName);

   /// opens service object
   Service OpenService(LPCTSTR pszServiceName);

private:
   /// handle to service control manager
   ServiceControlHandle m_spSCM;
};

} // namespace Win32
