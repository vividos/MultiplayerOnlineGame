//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ConsoleServerProgramOptions.hpp Program options for console server
//
#pragma once

// includes
#include <ulib/ProgramOptions.hpp>
#include <boost/bind.hpp>

/// options for server
class ConsoleServerProgramOptions: public ProgramOptions
{
public:
   /// ctor
   ConsoleServerProgramOptions()
      :m_usPort(c_usDefaultServerPort)
   {
      RegisterOutputHandler(&ProgramOptions::OutputConsole);
      RegisterHelpOption();

      CString cszDescription;
      cszDescription.Format(_T("Port for game clients (default: %u)"), c_usDefaultServerPort);

      RegisterOption(_T("p"), _T("port"), cszDescription,
         std::bind(&ConsoleServerProgramOptions::ParsePort, this, std::placeholders::_1));
   }

   /// returns port
   unsigned short Port() const throw() { return m_usPort; }

private:
   /// parses port
   bool ParsePort(const CString& cszPort)
   {
      unsigned long ulPort = _tcstoul(cszPort, NULL, 10);
      if (ulPort >= 0x10000)
         return false;

      m_usPort = static_cast<unsigned short>(ulPort);
      return true;
   }

private:
   unsigned short m_usPort; ///< port
};
