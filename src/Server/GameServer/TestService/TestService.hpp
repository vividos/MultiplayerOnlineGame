//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2019 Michael Fink
//
/// \file TestService.hpp Test service for unit tests
//
#pragma once

// include
#include <ulib/thread/Event.hpp>
#include <ulib/Path.hpp>
#include "ServiceInfo.hpp"
#include "LogCategories.hpp"

/// test service
class TestService
{
public:
   /// ctor
   TestService()
      :m_evtStop(false)
   {
   }

   /// returns service info
   static Win32::ServiceInfo GetServiceInfo()
   {
      return Win32::ServiceInfo(
         _T("testservice"),
         _T("Test Service 123"),
         _T("Tests Service class"),
         Log::Server::Service,
         Path::ModuleFilename());
   }

   /// runs service
   void Run()
   {
      ATLTRACE(_T("TestService started\n"));
      m_evtStop.Wait(INFINITE);
      ATLTRACE(_T("TestService stopped\n"));
   }

   /// stops service; called on another thread
   void Stop()
   {
      ATLTRACE(_T("TestService stop request\n"));
      m_evtStop.Set();
   }

private:
   /// event to signal stop
   ManualResetEvent m_evtStop;
};
