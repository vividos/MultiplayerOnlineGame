//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SystemInfo.hpp System info functions
//
#pragma once

/// \brief system info functions
class SystemInfo
{
public:
   /// returns display name of OS
   static CString GetOSDisplayString();

   /// returns display name of compiler
   static CString GetCompilerNameDisplayString();

   /// returns display name of boost library
   static CString BoostLibraryVersion();

private:
   /// ctor; not implemented
   SystemInfo();
   /// dtor; not implemented
   ~SystemInfo();
};
