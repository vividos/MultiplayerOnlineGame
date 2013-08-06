//
// ulib - a collection of useful classes
// Copyright (C) 2011,2012 Michael Fink
//
/// \file CompileTimeWarningError.hpp Compile time warning and error macros
//
#pragma once

// from: http://support.microsoft.com/kb/155196/

/// converts its parameter to a string
#define ULIB_STR2(x) #x
/// converts its parameter to a string; indirection through ULIB_STR2
#define ULIB_STR1(x) ULIB_STR2(x)
/// file/line number text macro
#define ULIB_LOC_WARN __FILE__ "(" ULIB_STR1(__LINE__) ") : "

/// In source code use like this:
/// \code #pragma ULIB_COMPILE_WARN("Check warning here") \endcode
#define ULIB_COMPILE_WARN(s) message(ULIB_LOC_WARN "warning: " s)

/// In source code use like this:
/// \code #pragma ULIB_COMPILE_ERROR("C9999", "Don't forget to code here!") \endcode
#define ULIB_COMPILE_ERROR(d,s) message(ULIB_LOC_WARN "error " d ": " s)
