//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file LogFileAppender.hpp Log file appender
//
#pragma once

// includes
#include "Common.hpp"
#include <ulib/log/Appender.hpp>

/// \brief appender that writes to log file
/// \details log file is created with current date in name
class COMMON_DECLSPEC LogFileAppender : public Log::Appender
{
public:
   /// ctor
   LogFileAppender();
   /// dtor
   virtual ~LogFileAppender() throw() {}

   /// outputs logging event to log file
   virtual void DoAppend(const Log::LoggingEventPtr spLoggingEvent);

private:
   /// returns log file name
   static CString CreateLogFileName() throw();

private:
   /// log file
   std::shared_ptr<FILE> m_spLogFile;
};
