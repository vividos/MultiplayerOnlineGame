//
// ulib - a collection of useful classes
// Copyright (C) 2007,2008,2009,2012 Michael Fink
//
/// \file TextStreamAppender.hpp text stream appender class
//
#pragma once

// includes
#include <ulib/log/Appender.hpp>
#include <ulib/stream/ITextStream.hpp>

namespace Log
{

/// \brief appender that uses a text stream for output
/// \details outputs formatted text to the given stream
class TextStreamAppender: public Appender
{
public:
   /// ctor
   TextStreamAppender(boost::shared_ptr<Stream::ITextStream> spTextStream) throw()
      :m_spTextStream(spTextStream)
   {
   }

   /// dtor
   virtual ~TextStreamAppender() throw() {}

   /// outputs logging event using OutputDebugString
   virtual void DoAppend(const LoggingEventPtr spLoggingEvent)
   {
      ATLASSERT(Layout().get() != NULL);

      CString cszOutput;
      Layout()->Format(cszOutput, spLoggingEvent);

      m_spTextStream->Write(cszOutput);
      m_spTextStream->WriteEndline();
      m_spTextStream->Flush();
   }

private:
   boost::shared_ptr<Stream::ITextStream> m_spTextStream;
};


} // namespace Log
