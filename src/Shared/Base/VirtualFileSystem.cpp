//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file VirtualFileSystem.cpp Virtual file system
//

// includes
#include "StdAfx.h"
#include "VirtualFileSystem.hpp"
#include <ulib/stream/FileStream.hpp>
#include "ZipArchive.hpp"
#include "Path.hpp"

VirtualFileSystem::VirtualFileSystem() throw()
{
}

void VirtualFileSystem::AddArchive(const CString& cszFilename)
{
   std::shared_ptr<Stream::FileStream> spFile(
      new Stream::FileStream(cszFilename,
         Stream::FileStream::modeOpen,
         Stream::FileStream::accessRead,
         Stream::FileStream::shareRead));

   ZipArchive archive(spFile);

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

bool VirtualFileSystem::IsFileAvail(const CString& cszFilename) const
{
   if (m_mapFilenameToArchive.find(cszFilename) != m_mapFilenameToArchive.end())
      return true;

   return BaseFileSystem::IsFileAvail(Path::Combine(AppDataFolder(), cszFilename));
}

std::shared_ptr<Stream::IStream> VirtualFileSystem::OpenFile(const CString& cszFilename,
   bool bForReading)
{
   // writing? redirect to base
   if (!bForReading)
      return BaseClass::OpenFile(cszFilename, bForReading);

   if (m_mapFilenameToArchive.find(cszFilename) == m_mapFilenameToArchive.end())
   {
      // try opening real file
      return BaseClass::OpenFile(Path::Combine(BaseClass::AppDataFolder(), cszFilename), bForReading);
   }

   const ArchiveInfo& info = m_mapFilenameToArchive[cszFilename];

   ATLASSERT(info.m_uiArchiveIndex < m_vecArchives.size());

   std::shared_ptr<Stream::FileStream> spFile(
      new Stream::FileStream(m_vecArchives[info.m_uiArchiveIndex],
         Stream::FileStream::modeOpen,
         Stream::FileStream::accessRead,
         Stream::FileStream::shareRead));

   ZipArchive archive(spFile);

   /// returns a file in the archive as stream
   return archive.GetFile(info.m_uiInArchiveIndex);
}
