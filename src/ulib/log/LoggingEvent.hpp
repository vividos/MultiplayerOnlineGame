//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014 Michael Fink
//
/// \file LoggingEvent.hpp logging event class
//
#pragma once

// includes
#include <ulib/log/Log.hpp>
#include <ulib/DateTime.hpp>
#include <ulib/Thread.hpp>
#include <vector>

namespace Log
{

/// logging event
class LoggingEvent
{
public:
   /// ctor
   LoggingEvent(Log::Level enLevel, const CString& cszLoggerName, const CString& cszMessage,
      const CString& cszSourceFilename, UINT uiSourceLine)
      :m_enLevel(enLevel),
       m_cszLoggerName(cszLoggerName),
       m_cszMessage(cszMessage),
       m_cszSourceFilename(cszSourceFilename),
       m_uiSourceLine(uiSourceLine),
       m_dwThreadId(Thread::CurrentId()),
       m_dtTimestamp(DateTime::Now())
   {
   }

   /// log level
   Log::Level Level() const throw() { return m_enLevel; }

   /// logger name
   CString LoggerName() const throw() { return m_cszLoggerName; }

   /// message
   CString Message() const throw() { return m_cszMessage; }

   /// source filename
   CString SourceFilename() const throw() { return m_cszSourceFilename; }

   /// sourcefile line
   UINT SourceLine() const throw() { return m_uiSourceLine; }

   /// thread id
   DWORD ThreadId() const throw() { return m_dwThreadId; }

   /// timestamp of the event
   const DateTime& Timestamp() const throw() { return m_dtTimestamp; }

   /// parameter count
   size_t ParamCount() const throw() { return m_vecParams.size(); }

   /// returns parameter
   CString Param(unsigned int uiIndex) const;

private:
   /// level
   Log::Level m_enLevel;

   /// logger name
   CString m_cszLoggerName;

   /// log message
   CString m_cszMessage;

   /// source filename
   CString m_cszSourceFilename;

   /// sourcefile line
   UINT m_uiSourceLine;

   /// thread id
   DWORD m_dwThreadId;

   /// event timestamp
   DateTime m_dtTimestamp;

   /// message parameter
   std::vector<CString> m_vecParams;
};

} // namespace Log
