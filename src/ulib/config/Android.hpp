//
// ulib - a collection of useful classes
// Copyright (C) 2013-2015 Michael Fink
//
/// \file Android.hpp Android NDK config
//
#pragma once

// includes
#include <android/log.h>
#include <cassert>

// override is unknown on NDK
#define override

// Win32 types
typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned short USHORT;
typedef unsigned int DWORD;
typedef unsigned int UINT;
typedef unsigned long long ULONGLONG;
typedef signed long long LONGLONG;

typedef char CHAR;
typedef CHAR* LPSTR;
typedef const CHAR* LPCSTR;

typedef wchar_t WCHAR;
typedef WCHAR* LPWSTR;
typedef const WCHAR* LPCWSTR;

#ifdef _UNICODE
typedef WCHAR TCHAR;
#define _T(x) L##x
#else
typedef CHAR TCHAR;
#define _T(x) x
#endif

typedef TCHAR* LPTSTR;
typedef const TCHAR* LPCTSTR;

#ifdef _UNICODE
#define _tprintf wprintf
#define _tcsftime wcsftime
#else
#define _tprintf printf
#define _tcsftime strftime
#endif

// Win32 functions
inline LPCTSTR GetCommandLine() { return _T(""); }

// ATL stuff
#define ATLASSERT assert
#define ATLTRACE(...)
#ifdef _DEBUG
#define ATLVERIFY(x) ATLASSERT(x)
#else
#define ATLVERIFY(x) (void)(x)
#endif

#include "CString.hpp"
