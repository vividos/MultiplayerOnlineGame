//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Filesystem.cpp File system implementation
//

// includes
#include "StdAfx.h"
#include "Filesystem.hpp"
#include "Path.hpp"
#include <shlobj.h> // for CSIDL_FONTS

#pragma comment(lib, "shell32.lib") // for SHGetSpecialFolderPath

Filesystem::Filesystem()
:m_cszBaseFolder(Filesystem::ModuleFilename())
{
   // TODO replace with Path class calls
   // remove filename
   int iPos = m_cszBaseFolder.ReverseFind('\\');
   ATLASSERT(iPos != -1);
   m_cszBaseFolder = m_cszBaseFolder.Left(iPos+1);

   m_cszBaseFolder += _T("data\\");
}

CString Filesystem::ModuleFilename() throw()
{
   CString cszModuleFilename;

   ::GetModuleFileName(NULL, cszModuleFilename.GetBuffer(MAX_PATH), MAX_PATH);
   cszModuleFilename.ReleaseBuffer();

   return cszModuleFilename;
}

CString Filesystem::FontsFolder() throw()
{
   // returns windows fonts folder
   CString cszFolder;
   ATLVERIFY(TRUE == SHGetSpecialFolderPath(NULL, cszFolder.GetBuffer(MAX_PATH), CSIDL_FONTS, TRUE));
   cszFolder.ReleaseBuffer();

   Path::AddEndingBackslash(cszFolder);

   return cszFolder;
}

CString Filesystem::UserFolder()
{
   // get non-roaming app data folder
   CString cszFolder;
   ATLVERIFY(TRUE == SHGetSpecialFolderPath(NULL, cszFolder.GetBuffer(MAX_PATH), CSIDL_LOCAL_APPDATA, TRUE));
   cszFolder.ReleaseBuffer();

   Path::AddEndingBackslash(cszFolder);

   // TODO store elsewhere
   cszFolder += _T("MultiplayerOnlineGame");

   if (GetFileAttributes(cszFolder) == INVALID_FILE_ATTRIBUTES)
      ATLVERIFY(TRUE == CreateDirectory(cszFolder, NULL));

   cszFolder += _T("\\");

   return cszFolder;
}
