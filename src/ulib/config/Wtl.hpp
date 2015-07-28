//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014 Michael Fink
//
/// \file Wtl.hpp configuration for WTL 8 or higher
//
#pragma once

// needed includes
#include <ulib/config/Atl.hpp>

// min and max macros are used in atlcom.h, etc., so temporarily define them here
#ifndef min
#  define min(x,y) (x) < (y) ? (x) : (y) ///< temporary define of min()
#endif

#ifndef max
#  define max(x,y) (x) > (y) ? (x) : (y) ///< temporary define of min()
#endif

// ignore prefast warnings in WTL header files
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6001 6011 6387 6509 6518 28252)
#endif

// ignore warnings in WTL header files not ported to Visual Studio 2015
#pragma warning(push)
#pragma warning(disable: 4302) // 'type cast' : truncation from 'T1' to 'T2'
#pragma warning(disable: 4838) // conversion from 'T1' to 'T2' requires a narrowing conversion
#pragma warning(disable: 4091) // 'typedef ' : ignored on left of 'T1' when no variable is declared
#pragma warning(disable: 4458) // declaration of 'var1' hides class member

// WTL includes
#include <atlapp.h>
extern CAppModule _Module; ///< app module
#include <atlwin.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <shellapi.h> // needed for ShellExecute, used in atlctrlx.h
#include <atlctrlx.h>
#include <atlddx.h>

#pragma warning(pop)

#ifdef _PREFAST_
#pragma warning(pop)
#endif

// undef the macros so that std::min and std::max work as they should be
#undef min
#undef max

// add manifest for common controls 6
#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
