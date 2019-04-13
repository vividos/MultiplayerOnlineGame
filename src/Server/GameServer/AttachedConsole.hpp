//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AttachedConsole.hpp Attached console class
//
#pragma once

namespace Win32
{

/// \brief attached console
/// \details when run as Win32 application, provides access to console when started on a console
class AttachedConsole
{
public:
   /// ctor
   AttachedConsole();
   /// dtor
   ~AttachedConsole();

   /// prints prompt and detaches console
   void Detach();

   /// indicates if this process is started from console
   bool IsCalledFromConsole() const { return m_bCalledFromConsole; }

private:
   /// reads command line prompt from current cursor pos
   static void ReadPrompt(CString& cszPrompt);

   /// \brief sets up redirection for stdout
   /// \details code from http://www.halcyon.com/~ast/dload/guicon.htm
   static void SetupRedirect();

private:
   /// indicates if process was started from console
   bool m_bCalledFromConsole;

   /// prompt string
   CString m_cszPrompt;
};

} // namespace Win32
