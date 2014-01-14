//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014 Michael Fink
//
/// \file Appender.hpp appender class
//
#pragma once

// includes
#include <ulib/config/AutoLink.hpp>
#include <ulib/log/Log.hpp>
#include <ulib/log/Layout.hpp>

namespace Log
{

/// \brief appender base class
/// \details appender do the output using their set layout class
class ULIB_DECLSPEC Appender
{
public:
   /// ctor
   Appender(){}

   /// dtor
   virtual ~Appender() throw() {}

   /// returns layout object being used
   LayoutPtr Layout() throw() { return m_spLayout; }

   /// sets new layout object
   void Layout(LayoutPtr spLayout) throw() { m_spLayout = spLayout; }

   /// appends logging event to output
   virtual void DoAppend(const LoggingEventPtr spLoggingEvent) = 0;

private:
   /// layout object
   LayoutPtr m_spLayout;
};


/// \brief console appender
/// \details outputs formatted text to the stdout console stream
class ConsoleAppender: public Appender
{
public:
   /// dtor
   virtual ~ConsoleAppender() throw() {}

   /// outputs logging event using _tprintf
   virtual void DoAppend(const LoggingEventPtr spLoggingEvent)
   {
      ATLASSERT(Layout().get() != NULL);

      CString cszOutput;
      Layout()->Format(cszOutput, spLoggingEvent);

      _tprintf(_T("%s\n"), cszOutput.GetString());
   }
};


/// \brief appender that uses OutputDebugString
/// \details outputs formatted text to the debug console
class OutputDebugStringAppender: public Appender
{
public:
   /// dtor
   virtual ~OutputDebugStringAppender() throw() {}

   /// outputs logging event using OutputDebugString
   virtual void DoAppend(const LoggingEventPtr spLoggingEvent)
   {
      ATLASSERT(Layout().get() != NULL);

      CString cszOutput;
      Layout()->Format(cszOutput, spLoggingEvent);

      OutputDebugString(cszOutput);
      OutputDebugString(_T("\n"));
   }
};

} // namespace Log
