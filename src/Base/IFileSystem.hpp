//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file IFileSystem.hpp File system interface
//
#pragma once

// forward references
namespace Stream
{
class IStream;
}

/// file system interface
class IFileSystem
{
public:
   /// dtor
   virtual ~IFileSystem() throw() {}

   /// returns folder name of user data folder; writable
   virtual CString UserDataFolder() const = 0;

   /// returns folder name of machine wide application data folder; writable
   virtual CString AppDataFolder() const = 0;

   /// opens a file stream; filename may be relative or absolute
   virtual boost::shared_ptr<Stream::IStream> OpenFile(const CString& cszFilename,
      bool bForReading = true) = 0;
};
