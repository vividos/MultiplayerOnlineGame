//
// ulib - a collection of useful classes
// Copyright (C) 2012 Michael Fink
//
/// \file src\stdafx.h precompiled header support
//
#pragma once

// including this defines the highest available windows platform
#if _MSC_VER <= 1500
#include <SDKDDKVer.h>
#else
// to build for a previous version, use this include and define
#include <WinSDKVer.h>
#define _WIN32_WINNT _WIN32_WINNT_WINXP
#include <SDKDDKVer.h>
#endif

#include <ulib/config/Atl.hpp>
