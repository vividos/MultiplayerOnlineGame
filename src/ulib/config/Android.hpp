//
// ulib - a collection of useful classes
// Copyright (C) 2013 Michael Fink
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
typedef unsigned int DWORD;
typedef unsigned long long ULONGLONG;
typedef signed long long LONGLONG;

typedef char CHAR;
typedef const CHAR* LPCSTR;
typedef wchar_t WCHAR;
typedef const WCHAR* LPCWSTR;
typedef CHAR TCHAR;
typedef const TCHAR* LPCTSTR;

#define _T(x) x

struct TIME_ZONE_INFORMATION
{
   const char* DaylightName;
   const char* StandardName;
};

// minimal MFC string class
class CString
{
public:
   CString()
   {
   }
   CString(const char*)
   {
   }
};

// android logging

#define  LOG_TAG "Underworld"

inline void AndroidLog(android_LogPriority prio, LPCSTR pszText, LPCSTR pszaFile, UINT uiLine)
{
   __android_log_print(prio, "Underworld", pszText);
}

#define LOG_DEBUG(msg, cat)   AndroidLog(ANDROID_LOG_DEBUG, LOG_TAG, ##msg, __FILE__, __LINE__)
#define LOG_INFO(msg, cat)    AndroidLog(ANDROID_LOG_INFO, LOG_TAG, ##msg, __FILE__, __LINE__)
#define LOG_WARN(msg, cat)    AndroidLog(ANDROID_LOG_WARN, LOG_TAG, ##msg, __FILE__, __LINE__)
#define LOG_ERROR(msg, cat)   AndroidLog(ANDROID_LOG_ERROR, LOG_TAG, ##msg, __FILE__, __LINE__)
#define LOG_FATAL(msg, cat)   AndroidLog(ANDROID_LOG_FATAL, LOG_TAG, ##msg, __FILE__, __LINE__)

// trick to make declspec expand to nothing on android
#define __declspec(xxx)

// ATL stuff
#define ATLASSERT assert
#define ATLTRACE(...)
#ifdef _DEBUG
#define ATLVERIFY(x) ATLASSERT(x)
#else
#define ATLVERIFY(x) x
#endif
