//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file LogFileAppender.cpp Log file appender
//

// includes
#include "StdAfx.h"
#include "LogFileAppender.hpp"
#include "Filesystem.hpp"
#include "Path.hpp"
#include <ulib/DateTime.hpp>

LogFileAppender::LogFileAppender()
{
   CString cszLogFilename = CreateLogFileName();

   FILE* fd = NULL;
   _tfopen_s(&fd, cszLogFilename, _T("at")); // append to text file; seeks to end
   if (fd == NULL)
      throw Exception(_T("couldn't open log file: ") + cszLogFilename, __FILE__, __LINE__);

   m_spLogFile.reset(fd, fclose);
}

void LogFileAppender::DoAppend(const Log::LoggingEventPtr spLoggingEvent)
{
   ATLASSERT(Layout().get() != NULL);

   CString cszOutput;
   Layout()->Format(cszOutput, spLoggingEvent);

   _ftprintf(m_spLogFile.get(), _T("%s\n"), cszOutput.GetString());

   fflush(m_spLogFile.get());
}

CString LogFileAppender::CreateLogFileName() throw()
{
   CString cszFolderName = Path::Combine(Filesystem::UserFolder(), _T("log"));

   if (!Path(cszFolderName).FolderExists())
      CreateDirectory(cszFolderName, nullptr);

   DateTime today = DateTime::Today();
   return Path::Combine(cszFolderName, today.Format(_T("Log_%Y-%m-%d.txt")));
}
