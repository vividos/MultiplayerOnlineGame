//
// ulib - a collection of useful classes
// Copyright (C) 2007,2008,2009,2012 Michael Fink
//
/// \file Layout.hpp layout class
//
#pragma once

// includes
#include <ulib/config/AutoLink.hpp>
#include <ulib/log/Log.hpp>
#include <ulib/log/LoggingEvent.hpp>

namespace Log
{

/// \brief log layout base class
/// \details the layout class formats the logging event for output
class ULIB_DECLSPEC Layout
{
public:
   /// dtor
   virtual ~Layout() throw() {}

   /// appends header to output; override when necessary
   virtual void AppendHeader(CString& cszOutput) throw() { cszOutput; }

   /// appends footer to output; override when necessary
   virtual void AppendFooter(CString& cszOutput) throw() { cszOutput; }

   /// formats logging event to string
   virtual void Format(CString& cszOutput, const LoggingEventPtr spLoggingEvent) throw() = 0;
};

/// \brief simple layout
/// \details prints out a simple text containing the log level and the message
class SimpleLayout: public Layout
{
public:
   /// dtor
   virtual ~SimpleLayout() throw() {}

   /// formats the simple log message
   virtual void Format(CString& cszOutput, const LoggingEventPtr spLoggingEvent) throw()
   {
      ATLASSERT(spLoggingEvent.get() != NULL);

      switch (spLoggingEvent->Level())
      {
      case debug: cszOutput = _T("DEBUG"); break;
      case info:  cszOutput = _T("INFO "); break;
      case warn:  cszOutput = _T("WARN "); break;
      case error: cszOutput = _T("ERROR"); break;
      case fatal: cszOutput = _T("FATAL"); break;
      default:
         cszOutput = _T("unknown"); break;
      }

      cszOutput += _T(" - ");
      cszOutput += spLoggingEvent->Message();
   }
};

/// \brief pattern layout
/// \details Uses a format pattern string to format a log line from logging
/// events. A pattern string consists of text and pattern specifiers that start
/// with the % char and end with a pattern character, similar to the printf
/// notation. The following characters have meaning:
/// - c: outputs logger name
/// - d: date in ISO 8601 format
/// - F: source filename where log message occured
/// - l: not supported
/// - L: source file line where log message occured
/// - m: log message; not supported
/// - n: platform-specific newline character
/// - p: log level (priority)
/// - r: not supported
/// - t: thread id
/// - %: percent sign
///
/// The following format modifiers are allowed:
/// - The - adds left justification to the string
/// - Next comes the minimum field width (excess space is padded)
/// - Optional . (dot)
/// - Next comes the maximum field width; if string is larger, the last n characters are shown
class ULIB_DECLSPEC PatternLayout: public Layout
{
public:
   /// ctor
   PatternLayout(const CString& cszPattern)
      :m_cszPattern(cszPattern)
   {
   }

   /// dtor
   virtual ~PatternLayout() throw() {}

   /// formats output using pattern
   virtual void Format(CString& cszOutput, const LoggingEventPtr spLoggingEvent) throw();

private:
   /// pattern
   CString m_cszPattern;
};

} // namespace Log
