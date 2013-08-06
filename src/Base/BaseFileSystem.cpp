//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file BaseFileSystem.cpp Base file system
//

// includes
#include "StdAfx.h"
#include "BaseFileSystem.hpp"
#include "Filesystem.hpp"
#include <ulib/stream/FileStream.hpp>

CString BaseFileSystem::UserDataFolder() const
{
   // TODO return user account data folder
   return Filesystem().BaseFolder();
}

CString BaseFileSystem::AppDataFolder() const
{
#ifdef _DEBUG
   return Filesystem().BaseFolder();
#else
   // TODO return Windows AppData folder
   return Filesystem().BaseFolder();
#endif
}

boost::shared_ptr<Stream::IStream> BaseFileSystem::OpenFile(const CString& cszFilename, bool bForReading)
{
   // TODO check that the path is inside the app data folder

   using Stream::FileStream;

   boost::shared_ptr<Stream::IStream> spStream(
      new Stream::FileStream(cszFilename,
         bForReading ? Stream::FileStream::modeOpen : Stream::FileStream::modeCreate,
         bForReading ? Stream::FileStream::accessRead : Stream::FileStream::accessWrite,
         Stream::FileStream::shareRead // always disallow writing
      ));

   return spStream;
}
