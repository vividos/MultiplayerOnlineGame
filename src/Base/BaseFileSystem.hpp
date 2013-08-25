//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file BaseFileSystem.hpp Base file system
//
#pragma once

// includes
#include "IFileSystem.hpp"

/// base file system
class BASE_DECLSPEC BaseFileSystem: public IFileSystem
{
public:
   /// dtor
   virtual ~BaseFileSystem() throw() {}

   /// returns folder name of user data folder; writable
   virtual CString UserDataFolder() const override;

   /// returns folder name of machine wide application data folder; writable
   virtual CString AppDataFolder() const override;

   /// opens a file stream; filename may be relative or absolute
   virtual std::shared_ptr<Stream::IStream> OpenFile(const CString& cszFilename,
      bool bForReading = true) override;
};
