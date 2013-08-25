//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file VirtualFileSystem.hpp Virtual file system
//
#pragma once

// includes
#include "Base.hpp"
#include "BaseFileSystem.hpp"

// forward references
namespace Stream
{
class IStream;
}

/// virtual file system using zip archives
class BASE_DECLSPEC VirtualFileSystem: public BaseFileSystem
{
   /// base class typedef
   typedef BaseFileSystem BaseClass;

public:
   DEFINE_INSTANCE(VirtualFileSystem)

   /// ctor
   VirtualFileSystem() throw();

   /// adds another archive to the VFS
   void AddArchive(const CString& cszFilename);

   /// opens a file for reading
   virtual std::shared_ptr<Stream::IStream> OpenFile(const CString& cszFilename,
      bool bForReading = true) override;

private:
   /// archive filenames
   std::vector<CString> m_vecArchives;

   /// archive info
   struct ArchiveInfo
   {
      /// index of arcive
      size_t m_uiArchiveIndex;

      /// index of file inside archive
      unsigned int m_uiInArchiveIndex;
   };

   /// mapping of lowercase filename to archive
   std::map<CString, ArchiveInfo> m_mapFilenameToArchive;
};
