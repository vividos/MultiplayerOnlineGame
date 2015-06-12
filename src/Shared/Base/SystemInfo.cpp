//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SystemInfo.cpp System info functions
//

// includes
#include "StdAfx.h"
#include "SystemInfo.hpp"
#include <ulib/SystemException.hpp>
#include <boost/config.hpp> // for BOOST_STRINGIZE
#include <boost/version.hpp> // for BOOST_VERSION
#include <VersionHelpers.h>

/// \details adapted from http://msdn.microsoft.com/en-us/library/windows/desktop/ms724429%28v=vs.85%29.aspx
CString SystemInfo::GetOSDisplayString()
{
   if (!IsWindowsXPOrGreater())
      return _T("unsupported Windows version");

   CString cszOS = _T("Microsoft ");

   // Test for the specific product.
   bool bServer = IsWindowsServer();

   if (IsWindows8Point1OrGreater())
   {
      if (!bServer)
         cszOS += TEXT("Windows 8.1 (or greater)");
      else
         cszOS += TEXT("Windows Server 2012 R2 (or greater)");
   }
   else
      if (IsWindows8OrGreater())
      {
         if (!bServer)
            cszOS += TEXT("Windows 8");
         else
            cszOS += TEXT("Windows Server 2012");
      }
      else
         if (IsWindows7SP1OrGreater())
         {
            if (!bServer)
               cszOS += TEXT("Windows 7 SP1");
            else
               cszOS += TEXT("Windows Server 2008 R2 SP1");
         }
         else
            if (IsWindows7OrGreater())
            {
               if (!bServer)
                  cszOS += TEXT("Windows 7");
               else
                  cszOS += TEXT("Windows Server 2008 R2");
            }
            else
               if (IsWindowsVistaOrGreater())
               {
                  if (!bServer)
                     cszOS += TEXT("Windows Vista");
                  else
                     cszOS += TEXT("Windows Server 2008");
               }
               else
                  if (IsWindowsXPOrGreater())
                  {
                     if (!bServer)
                        cszOS += TEXT("Windows XP");
                     else
                        cszOS += TEXT("Windows Server 2003");
                  }
                  else
                     cszOS += TEXT("Windows ???");

   return cszOS;
}

CString SystemInfo::GetCompilerNameDisplayString()
{
   CString cszVer;
#if defined(_MSC_VER)
   #if _MSC_VER > 1800
      cszVer = _T("Microsoft Visual C++ > 2013");
   #elif _MSC_VER == 1800
      cszVer = _T("Microsoft Visual C++ 2013");
   #elif _MSC_VER >= 1700
      cszVer = _T("Microsoft Visual C++ 2012");
   #elif _MSC_VER >= 1600
      cszVer = _T("Microsoft Visual C++ 2010");
   #elif _MSC_VER >= 1500
      cszVer = _T("Microsoft Visual C++ 2008");
   #elif _MSC_VER >= 1400
      cszVer = _T("Microsoft Visual C++ 2005");
   #elif _MSC_VER >= 1310
      cszVer = _T("Microsoft Visual C++ .NET 2003");
   #elif _MSC_VER >= 1300
      cszVer = _T("Microsoft Visual C++ .NET 2002");
   #elif _MSC_VER >= 1200
      cszVer = _T("Microsoft Visual C++ 6.0");
   #else
      cszVer = _T("Microsoft Visual C++ 5.0 or lower");
   #endif

   cszVer += _T(" (");
   USES_CONVERSION;
   cszVer += A2CT(BOOST_STRINGIZE(_MSC_VER));
   cszVer += _T(")");

#elif defined(__GNUC__)
   cszVer = CStringConversion::ConvertToTString(
      "gcc " BOOST_STRINGIZE(__GNUC__) "."
      BOOST_STRINGIZE(__GNUC_MINOR__) "."
      BOOST_STRINGIZE(__GNUC_PATCHLEVEL__));
#else
#error SystemInfo.cpp not ported to other compilers!
#endif

   return cszVer;
}

CString SystemInfo::BoostLibraryVersion()
{
   CString cszVer;
   cszVer.Format(_T("Boost %u.%u.%u"),
      (BOOST_VERSION / 100000),
      ((BOOST_VERSION / 100) % 1000),
      (BOOST_VERSION % 100) );
   return cszVer;
}
