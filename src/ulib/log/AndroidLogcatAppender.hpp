//
// ulib - a collection of useful classes
// Copyright (C) 2006-2015 Michael Fink
//
/// \file AndroidLogcatAppender.hpp appender that is using Android logcat
//
#pragma once

// includes
#include <ulib/config/AutoLink.hpp>
#include <ulib/log/Appender.hpp>

namespace Log
{

/// \brief appender that uses Android logcat logging
class AndroidLogcatAppender : public Appender
{
public:
   /// dtor
   virtual ~AndroidLogcatAppender() throw() {}

   /// outputs logging event using OutputDebugString
   virtual void DoAppend(const LoggingEventPtr spLoggingEvent)
   {
      ATLASSERT(Layout().get() != NULL);

      CString cszOutput;
      Layout()->Format(cszOutput, spLoggingEvent);

      android_LogPriority prio = ANDROID_LOG_DEBUG;

      switch (spLoggingEvent->Level())
      {
      case Log::Level::debug: prio = ANDROID_LOG_DEBUG; break;
      case Log::Level::info: prio = ANDROID_LOG_INFO; break;
      case Log::Level::warn: prio = ANDROID_LOG_WARN; break;
      case Log::Level::error: prio = ANDROID_LOG_ERROR; break;
      case Log::Level::fatal: prio = ANDROID_LOG_FATAL; break;
      default:
         ATLASSERT(false);
         break;
      }

#ifdef __ANDROID__
      __android_log_print(prio, spLoggingEvent->LoggerName().GetString(), cszOutput.GetString());
#endif
   }
};
#endif
