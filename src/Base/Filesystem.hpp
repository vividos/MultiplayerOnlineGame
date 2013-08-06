//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Filesystem.hpp File system
//
#pragma once

// includes
#include "Base.hpp"

/// file system
class BASE_DECLSPEC Filesystem
{
public:
   /// ctor
   Filesystem();

   /// returns module filename
   static CString ModuleFilename() throw();

   /// returns file system base folder, including trailing slash
   CString BaseFolder() const throw() { return m_cszBaseFolder; }

   /// returns database script folder, including trailing slash
   CString DatabaseScriptFolder() const throw() { return BaseFolder() + _T("script\\"); }

   /// returns a per-user path to a (writable) folder; creates folder if it doesn't exist
   static CString UserFolder();

   /// returns fonts folder
   static CString FontsFolder() throw();

private:
   /// base folder
   CString m_cszBaseFolder;
};
