//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ServiceProgramOptions.hpp Program options for service
//
#pragma once

// includes
#include <ulib/ProgramOptions.hpp>

namespace Win32
{

/// program options for win32 service
class ServiceProgramOptions: public ProgramOptions
{
public:
   /// service action
   enum T_enServiceAction
   {
      actionRun,        ///< runs service instance
      actionStart,      ///< starts service
      actionStop,       ///< stops service
      actionRegister,   ///< registers service
      actionUnregister, ///< unregisters service
      actionTest,       ///< runs service class directly; used for debugging
   };

   /// ctor
   ServiceProgramOptions()
      :m_enServiceAction(actionRun)
   {
      RegisterOption(_T("s"), _T("start"), _T("Starts service instance"), 0,
         std::bind(&ServiceProgramOptions::SetServiceAction, this, actionStart));

      RegisterOption(_T(""), _T("stop"), _T("Stops service instance"), 0,
         std::bind(&ServiceProgramOptions::SetServiceAction, this, actionStop));

      RegisterOption(_T("r"), _T("register"), _T("Registers service instance"), 0,
         std::bind(&ServiceProgramOptions::SetServiceAction, this, actionRegister));

      RegisterOption(_T("u"), _T("unregister"), _T("Unregisters service instance"), 0,
         std::bind(&ServiceProgramOptions::SetServiceAction, this, actionUnregister));

      RegisterOption(_T("t"), _T("test"), _T("Tests service"), 0,
         std::bind(&ServiceProgramOptions::SetServiceAction, this, actionTest));
   }

   /// returns service action
   T_enServiceAction ServiceAction() const throw() { return m_enServiceAction; }

private:
   /// sets service action; used in RegisterOption() call
   bool SetServiceAction(T_enServiceAction enServiceAction) throw()
   {
      m_enServiceAction = enServiceAction;
      return true;
   }

private:
   /// service action
   T_enServiceAction m_enServiceAction;
};

} // namespace Win32
