//
// ulib - a collection of useful classes
// Copyright (C) 2007,2008,2012 Michael Fink
//
/// \file Logger.cpp Logger implementation
//

// needed includes
#include "stdafx.h"
#include <ulib/log/Logger.hpp>
#include <ulib/log/Appender.hpp>
#include <ulib/log/LoggingEvent.hpp>

#define BOOST_THREAD_NO_LIB
#include <boost/thread/once.hpp>

using Log::Logger;
using Log::LoggerPtr;

static LONG s_uiLoggerInitialized = 0;

CString Logger::Name() throw()
{
   // check if parent is the root logger
   if (Parent() == NULL)
      return _T("");

   if (Parent()->Parent() == NULL)
      return m_cszName; // just one above the parent
   else
      return Parent()->Name() + _T(".") + m_cszName;
}

boost::once_flag g_rootLoggerOnceFlag = BOOST_ONCE_INIT;

static Log::LoggerPtr s_spRootLogger;

void Logger::InitRootLogger()
{
   s_spRootLogger = Log::LoggerPtr(new Log::Logger(_T(""), Log::LoggerPtr()));
}

/// \todo implement MT
LoggerPtr Logger::GetRootLogger()
{
   boost::call_once(g_rootLoggerOnceFlag, &Logger::InitRootLogger);
   return s_spRootLogger;
}

LoggerPtr Logger::GetLogger(const CString& cszName)
{
   LoggerPtr spLogger = GetRootLogger();
   ATLASSERT(spLogger != NULL); // should not happen!

   // recursively search logger map for logger
   int iPos = 0, iPosMax = cszName.GetLength();
   while (iPos < iPosMax)
   {
      CString cszLoggerName;

      // check if there's a dot
      int iPosDot = cszName.Find(_T('.'), iPos);
      if (iPosDot != -1)
      {
         cszLoggerName = cszName.Mid(iPos, iPosDot-iPos);
         iPos = iPosDot+1;
      }
      else
      {
         // no further dots
         cszLoggerName = cszName.Mid(iPos);
         iPos = iPosMax;
      }

      cszLoggerName.MakeLower();

      // search logger name
      T_mapLoggerMap::const_iterator iter = spLogger->m_mapChildLogger.find(cszLoggerName);
      if (iter == spLogger->m_mapChildLogger.end())
      {
         // no logger; create it
         LoggerPtr spNewLogger(new Logger(cszLoggerName, spLogger));
         spLogger->m_mapChildLogger.insert(std::make_pair(cszLoggerName, spNewLogger));
         spLogger = spNewLogger;
      }
      else
         // found logger; use as next (or last)
         spLogger = iter->second;
   }

   return spLogger;
}

void Logger::Log(Log::Level enLevel, const CString& cszMessage, LPCSTR pszFilename, UINT uiLine)
{
   ATLASSERT(enLevel != none);

   // find out logger level
   Log::Level enLoggerLevel = GetLoggerLevel();

   if (enLevel >= enLoggerLevel)
   {
      // send message to all appenders
      CString cszSourceFilename(pszFilename);

      LoggingEventPtr spEvent(
         new LoggingEvent(enLevel, Name(), cszMessage, cszSourceFilename, uiLine));

      LogEvent(spEvent);
   }
}

void Logger::LogEvent(Log::LoggingEventPtr spEvent)
{
   CallAppenders(spEvent);

   // when logger is additive (and not a root logger), send to parent
   if (m_bAdditivity && m_spParent.get() != NULL)
      Parent()->LogEvent(spEvent);
}

Logger::Logger(const CString& cszName, LoggerPtr spParent)
:m_spParent(spParent),
 m_cszName(cszName),
 m_enLevel(none),
 m_bAdditivity(true)
{
   // construct a root logger?
   if (spParent.get() == NULL)
   {
      m_enLevel = debug;
      m_bAdditivity = false;
   }
}

Log::Level Logger::GetLoggerLevel()
{
   Log::Level enLoggerLevel = m_enLevel;
   if (enLoggerLevel == none)
   {
      LoggerPtr spParent = m_spParent;
      do
      {
         ATLASSERT(spParent.get() != NULL);
         enLoggerLevel = spParent->Level();
         spParent = spParent->Parent();
      } while(enLoggerLevel == none);
   }

   return enLoggerLevel;
}

void Logger::CallAppenders(const Log::LoggingEventPtr spEvent)
{
   T_setAppender::const_iterator iter = m_setAppender.begin(), stop = m_setAppender.end();
   for (; iter != stop; ++iter)
      (*iter)->DoAppend(spEvent);
}
