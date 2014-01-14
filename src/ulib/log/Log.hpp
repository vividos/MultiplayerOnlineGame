//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014 Michael Fink
//
/// \file Log.hpp logging
//
#pragma once

// includes
#include <memory>

/// \brief Logging facilities
namespace Log
{

/// log level
enum Level
{
   debug=0, ///< debug; used only for debugging purposes
   info,    ///< informal log message
   warn,    ///< warning log message
   error,   ///< error log message
   fatal,   ///< fatal error log message
   none,    ///< no log level
};

/// logger smart pointer
typedef std::shared_ptr<class Logger> LoggerPtr;

/// layout smart pointer
typedef std::shared_ptr<class Layout> LayoutPtr;

/// appender smart pointer
typedef std::shared_ptr<class Appender> AppenderPtr;

/// logging event smart pointer
typedef std::shared_ptr<class LoggingEvent> LoggingEventPtr;

} // namespace Log
