//
// ulib - a collection of useful classes
// Copyright (C) 2006-2015 Michael Fink
//
/// \file OutputDebugStringAppender.hpp appender that is using OutputDebugString()
//
#pragma once

// includes
#include <ulib/config/AutoLink.hpp>
#include <ulib/log/Appender.hpp>

namespace Log
{
   /// \brief appender that uses OutputDebugString
   /// \details outputs formatted text to the debug console
   class OutputDebugStringAppender : public Appender
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

#ifdef _MSC_VER
         OutputDebugString(cszOutput);
         OutputDebugString(_T("\n"));
#endif
      }
   };

} // namespace Log
