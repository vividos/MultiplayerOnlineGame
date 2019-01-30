//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Base.hpp Base project header
//
#pragma once

// includes
#include "targetver.h"
#include <boost/noncopyable.hpp>

// Win32
#ifdef WIN32
#include <ulib/config/Atl.hpp>
// Android
#elif defined(__ANDROID__)
#include <ulib/config/Android.hpp>
#endif

#include <ulib/Exception.hpp>
#include <ulib/log/Logger.hpp>
#include <ulib/config/BoostAsio.hpp>

#include <memory>
#include <functional>

/// export macro
#ifdef BASE_EXPORTS
#  define BASE_DECLSPEC __declspec(dllexport)
#else
#  ifdef UNIT_TEST
#    define BASE_DECLSPEC
#  else
#    define BASE_DECLSPEC __declspec(dllimport)
#  endif
#endif

// ignore common warnings

#pragma warning(disable: 4251) // 'T1' : class 'T2' needs to have dll-interface to be used by clients of class 'T3'
#pragma warning(disable: 4275) // non dll-interface class 'T1' used as base for dll-interface class 'T2'
#pragma warning(disable: 4512) // 'T' : assignment operator could not be generated
#pragma warning(disable: 4481) // nonstandard extension used: override specifier 'override'

#undef min
#undef max

/// macro to define class that can be managed with InstanceManager
#define DEFINE_INSTANCE(ClassName) \
   static LPCTSTR GetClassName() throw() { return _T(#ClassName); }

// when running cppcheck, ignore throw() and override specifier
#ifdef CPPCHECK
#define throw()
#define override
#endif

// provide defines when doxygen is generating docs
#ifdef DOXYGEN
#endif
