//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file VirtualFileSystem.cpp Virtual file system
//

// includes
#include "StdAfx.h"
#include "VirtualFileSystem.hpp"
#include <ulib/stream/FileStream.hpp>
#include "ZipArchive.hpp"

VirtualFileSystem::VirtualFileSystem() throw()
{
}

void VirtualFileSystem::AddArchive(const CString& cszFilename)
{
   Stream::FileStream fs(cszFilename,
      Stream::FileStream::modeOpen,
      Stream::FileStream::accessRead,
      Stream::FileStream::shareRead);

   ZipArchive archive(fs);

   size_t iArchiveIndex = m_vecArchives.size();
   m_vecArchives.push_back(cszFilename);

   ArchiveInfo info;
   info.m_uiArchiveIndex = iArchiveIndex;

   CString cszArchiveFilename;
   for (unsigned int i=0, iMax=archive.FileCount(); i<iMax; i++)
   {
      cszArchiveFilename = archive.Filename(i, true);
      cszArchiveFilename.MakeLower();

      info.m_uiInArchiveIndex = i;
      m_mapFilenameToArchive.insert(std::make_pair(cszArchiveFilename, info));
   }
}

boost::shared_ptr<Stream::IStream> VirtualFileSystem::OpenFile(const CString& cszFilename,
   bool bForReading)
{
   // writing? redirect to base
   if (!bForReading)
      return BaseClass::OpenFile(cszFilename, bForReading);

   if (m_mapFilenameToArchive.find(cszFilename) == m_mapFilenameToArchive.end())
   {
      // try opening real file
      return BaseClass::OpenFile(cszFilename, bForReading);
   }

   const ArchiveInfo& info = m_mapFilenameToArchive[cszFilename];

   ATLASSERT(info.m_uiArchiveIndex < m_vecArchives.size());

   Stream::FileStream fs(m_vecArchives[info.m_uiArchiveIndex],
      Stream::FileStream::modeOpen,
      Stream::FileStream::accessRead,
      Stream::FileStream::shareRead);

   ZipArchive archive(fs);

   /// returns a file in the archive as stream
   return archive.GetFile(info.m_uiInArchiveIndex);
}
