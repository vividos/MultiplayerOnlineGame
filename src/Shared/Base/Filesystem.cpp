//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Filesystem.cpp File system implementation
//

// includes
#include "StdAfx.h"
#include "Filesystem.hpp"
#include <ulib/Path.hpp>
#include <shlobj.h> // for CSIDL_FONTS

Filesystem::Filesystem()
:m_cszBaseFolder(Path::Combine(Path::ModuleFilename(), _T("ClientData")))
{
}

CString Filesystem::FontsFolder() throw()
{
   // returns windows fonts folder
   CString cszFolder = Path::SpecialFolder(CSIDL_FONTS);

   Path::AddEndingBackslash(cszFolder);

   return cszFolder;
}

CString Filesystem::UserFolder()
{
   // get non-roaming app data folder
   CString cszFolder = Path::SpecialFolder(CSIDL_LOCAL_APPDATA);

   Path::AddEndingBackslash(cszFolder);

   // TODO store elsewhere
   cszFolder += _T("MultiplayerOnlineGame");

   if (GetFileAttributes(cszFolder) == INVALID_FILE_ATTRIBUTES)
      ATLVERIFY(TRUE == CreateDirectory(cszFolder, NULL));

   cszFolder += _T("\\");

   return cszFolder;
}
