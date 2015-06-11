//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ServiceRunner.hpp Win32 Service runner
//
#pragma once

// includes
#include "ServiceInstance.hpp"

namespace Win32
{

/// service runner for one or more services
class ServiceRunner
{
public:
   /// \brief adds service to run
   /// \tparam TService class to add as service object
   template <typename TService>
   void AddService()
   {
      m_vecAllServices.push_back(ServiceInstance<TService>::m_serviceTableEntry);
   }

   /// runs all added services by calling SCM
   void Run()
   {
      // add an "end" dummy entry
      SERVICE_TABLE_ENTRY endEntry = {0};
      m_vecAllServices.push_back(endEntry);

      BOOL bRet = ::StartServiceCtrlDispatcher(&m_vecAllServices[0]);
      if (bRet == FALSE)
         throw SystemException(_T("StartServiceCtrlDispatcher"), GetLastError(), __FILE__, __LINE__);
   }

private:
   /// all service table entries
   std::vector<SERVICE_TABLE_ENTRY> m_vecAllServices;
};

} // namespace Win32
