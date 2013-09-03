//
// ulib - a collection of useful classes
// Copyright (C) 2007-2013 Michael Fink
//
/// \file Logger.hpp logger class
//
#pragma once

// includes
#include <ulib/config/AutoLink.hpp>
#include <ulib/log/Log.hpp>
#include <memory>
#include <map>
#include <set>

namespace Log
{
/// logging event smart pointer
typedef std::shared_ptr<class LoggingEvent> LoggingEventPtr;

/// logger
class ULIB_DECLSPEC Logger
{
public:
   // properties

   /// returns logger level
   Level Level() const throw() { return m_enLevel; }
   /// returns additivity flag
   bool Additivity() const throw() { return m_bAdditivity; }
   /// returns parent
   LoggerPtr Parent() throw(){ return m_spParent; }
   /// returns full logger name
   CString Name() throw();

   /// sets logger level
   void Level(Log::Level enLevel) throw() { m_enLevel = enLevel; }

   /// sets additivity
   void Additivity(bool fAdditivity) throw() { m_bAdditivity = fAdditivity; }


   // methods

   /// returns root logger
   static LoggerPtr GetRootLogger();

   /// returns logger with given name
   static LoggerPtr GetLogger(const CString& cszName);

   /// adds appender to this logger
   void AddAppender(AppenderPtr spAppender)
   {
      m_setAppender.insert(spAppender);
   }

   /// removes all appender from this logger
   void RemoveAllAppender()
   {
      m_setAppender.clear();
   }

   /// removes given appender
   void RemoveAppender(AppenderPtr spAppender)
   {
      T_setAppender::iterator iter = m_setAppender.find(spAppender);
      if (iter != m_setAppender.end())
         m_setAppender.erase(iter);
   }

   /// logs a debug message
   void Debug(const CString& cszMessage, LPCSTR pszFilename, UINT uiLine)
   {
      Log(debug, cszMessage, pszFilename, uiLine);
   }

   /// logs an info message
   void Info(const CString& cszMessage, LPCSTR pszFilename, UINT uiLine)
   {
      Log(info, cszMessage, pszFilename, uiLine);
   }

   /// logs a warning message
   void Warn(const CString& cszMessage, LPCSTR pszFilename, UINT uiLine)
   {
      Log(warn, cszMessage, pszFilename, uiLine);
   }

   /// logs an error message
   void Error(const CString& cszMessage, LPCSTR pszFilename, UINT uiLine)
   {
      Log(error, cszMessage, pszFilename, uiLine);
   }

   /// logs a fatal error message
   void Fatal(const CString& cszMessage, LPCSTR pszFilename, UINT uiLine)
   {
      Log(fatal, cszMessage, pszFilename, uiLine);
   }

   /// logs a message with given level
   void Log(Log::Level enLevel, const CString& cszMessage, LPCSTR pszFilename, UINT uiLine);

   /// logs a logging event
   void LogEvent(LoggingEventPtr spEvent);

private:
   /// ctor; logger can only be created by itself
   Logger(const CString& cszName, LoggerPtr spParent);

   /// inits root logger
   static void InitRootLogger();

   /// determines logger level; asks parent if no level was set
   Log::Level GetLoggerLevel();

   /// call all appenders using given event
   void CallAppenders(const LoggingEventPtr spEvent);

private:
   /// logger level
   Log::Level m_enLevel;

   /// additivity; when true, log events are sent to parent, too
   bool m_bAdditivity;

   /// parent
   LoggerPtr m_spParent;

   /// logger name
   CString m_cszName;

   /// map type for child logger
   typedef std::map<CString, LoggerPtr> T_mapLoggerMap;
   /// logger name / child logger mapping
   T_mapLoggerMap m_mapChildLogger;

   /// set type containing appender
   typedef std::set<AppenderPtr> T_setAppender;
   /// set with all appenders for this logger
   T_setAppender m_setAppender;
};

}

/// logs using DEBUG level
#define LOG_DEBUG(msg, cat) ::Log::Logger::GetLogger(cat)->Debug(msg, __FILE__, __LINE__);
/// logs using INFO level
#define LOG_INFO(msg, cat) ::Log::Logger::GetLogger(cat)->Info(msg, __FILE__, __LINE__);
/// logs using WARN level
#define LOG_WARN(msg, cat) ::Log::Logger::GetLogger(cat)->Warn(msg, __FILE__, __LINE__);
/// logs using ERROR level
#define LOG_ERROR(msg, cat) ::Log::Logger::GetLogger(cat)->Error(msg, __FILE__, __LINE__);
/// logs using FATAL level
#define LOG_FATAL(msg, cat) ::Log::Logger::GetLogger(cat)->Fatal(msg, __FILE__, __LINE__);
