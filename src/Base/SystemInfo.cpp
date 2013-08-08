//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file SystemInfo.cpp System info functions
//

// includes
#include "StdAfx.h"
#include "SystemInfo.hpp"
#include <ulib/SystemException.hpp>
#include <strsafe.h>
#include <boost/config.hpp> // for BOOST_STRINGIZE
#include <boost/version.hpp> // for BOOST_VERSION

/// from MSDN article "Getting the System Version", on
/// http://msdn.microsoft.com/en-us/library/ms724429.aspx

#ifndef PRODUCT_PROFESSIONAL
#define PRODUCT_PROFESSIONAL 0x00000030
#endif

#ifndef VER_SUITE_WH_SERVER
/// define for windows home server
#define VER_SUITE_WH_SERVER 0x8000
#endif

#pragma comment(lib, "user32.lib")

/// function type for GetNativeSystemInfo()
typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);

/// function type for GetProductInfo()
typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

/// \details adapted from http://msdn.microsoft.com/en-us/library/windows/desktop/ms724429%28v=vs.85%29.aspx
CString SystemInfo::GetOSDisplayString()
{
   OSVERSIONINFOEX osvi;
   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
   BOOL bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi);

   if (!bOsVersionInfoEx)
      throw SystemException(_T("GetVersionEx"), GetLastError(), __FILE__, __LINE__);

   // Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

   PGNSI pGNSI = (PGNSI) GetProcAddress(
      GetModuleHandle(TEXT("kernel32.dll")),
      "GetNativeSystemInfo");

   SYSTEM_INFO si;
   ZeroMemory(&si, sizeof(SYSTEM_INFO));

   if (NULL != pGNSI)
      pGNSI(&si);
   else
      GetSystemInfo(&si);

   if (VER_PLATFORM_WIN32_NT != osvi.dwPlatformId || osvi.dwMajorVersion <= 4)
      return _T("unsupported Windows version");

   CString cszOS(TEXT("Microsoft "));

   // Test for the specific product.

   if (osvi.dwMajorVersion == 6)
   {
      if (osvi.dwMinorVersion == 0)
      {
         if (osvi.wProductType == VER_NT_WORKSTATION)
            cszOS += TEXT("Windows Vista ");
         else
            cszOS += TEXT("Windows Server 2008 ");
      }

      if (osvi.dwMinorVersion == 1 || osvi.dwMinorVersion == 2 )
      {
         if (osvi.wProductType == VER_NT_WORKSTATION)
         {
            if (osvi.dwMinorVersion == 1)
               cszOS += TEXT("Windows 7 ");
            else if (osvi.dwMinorVersion == 2)
               cszOS += TEXT("Windows 8 ");
            else if (osvi.dwMinorVersion == 3)
               cszOS += TEXT("Windows Server 8.1 ");
            else
               cszOS += TEXT("Windows ??? ");
         }
         else
         {
            if (osvi.dwMinorVersion == 1)
               cszOS += TEXT("Windows Server 2008 R2 ");
            else if (osvi.dwMinorVersion == 2)
               cszOS += TEXT("Windows Server 2012 ");
            else
               cszOS += TEXT("Windows Server ??? ");
         }
      }

      PGPI pGPI = (PGPI) GetProcAddress(
         GetModuleHandle(TEXT("kernel32.dll")),
         "GetProductInfo");

      DWORD dwType = PRODUCT_UNDEFINED;
      pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.wServicePackMajor, osvi.wServicePackMinor, &dwType);

      switch (dwType)
      {
      case PRODUCT_ULTIMATE:
         cszOS += TEXT("Ultimate Edition");
         break;
      case PRODUCT_PROFESSIONAL:
         cszOS += TEXT("Professional");
         break;
      case PRODUCT_HOME_PREMIUM:
         cszOS += TEXT("Home Premium Edition");
         break;
      case PRODUCT_HOME_BASIC:
         cszOS += TEXT("Home Basic Edition");
         break;
      case PRODUCT_ENTERPRISE:
         cszOS += TEXT("Enterprise Edition");
         break;
      case PRODUCT_BUSINESS:
         cszOS += TEXT("Business Edition");
         break;
      case PRODUCT_STARTER:
         cszOS += TEXT("Starter Edition");
         break;
      case PRODUCT_CLUSTER_SERVER:
         cszOS += TEXT("Cluster Server Edition");
         break;
      case PRODUCT_DATACENTER_SERVER:
         cszOS += TEXT("Datacenter Edition");
         break;
      case PRODUCT_DATACENTER_SERVER_CORE:
         cszOS += TEXT("Datacenter Edition (core installation)");
         break;
      case PRODUCT_ENTERPRISE_SERVER:
         cszOS += TEXT("Enterprise Edition");
         break;
      case PRODUCT_ENTERPRISE_SERVER_CORE:
         cszOS += TEXT("Enterprise Edition (core installation)");
         break;
      case PRODUCT_ENTERPRISE_SERVER_IA64:
         cszOS += TEXT("Enterprise Edition for Itanium-based Systems");
         break;
      case PRODUCT_SMALLBUSINESS_SERVER:
         cszOS += TEXT("Small Business Server");
         break;
      case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
         cszOS += TEXT("Small Business Server Premium Edition");
         break;
      case PRODUCT_STANDARD_SERVER:
         cszOS += TEXT("Standard Edition");
         break;
      case PRODUCT_STANDARD_SERVER_CORE:
         cszOS += TEXT("Standard Edition (core installation)");
         break;
      case PRODUCT_WEB_SERVER:
         cszOS += TEXT("Web Server Edition");
         break;
      }
   }

   if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
   {
      if (GetSystemMetrics(SM_SERVERR2))
         cszOS += TEXT( "Windows Server 2003 R2, ");
      else if (osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER)
         cszOS += TEXT( "Windows Storage Server 2003");
      else if (osvi.wSuiteMask & VER_SUITE_WH_SERVER)
         cszOS += TEXT( "Windows Home Server");
      else if (osvi.wProductType == VER_NT_WORKSTATION &&
         si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
      {
         cszOS += TEXT( "Windows XP Professional x64 Edition");
      }
      else cszOS += TEXT("Windows Server 2003, ");

      // Test for the server type.
      if (osvi.wProductType != VER_NT_WORKSTATION)
      {
         if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
         {
            if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
               cszOS += TEXT( "Datacenter Edition for Itanium-based Systems");
            else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
               cszOS += TEXT( "Enterprise Edition for Itanium-based Systems");
         }

         else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
         {
            if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
               cszOS += TEXT( "Datacenter x64 Edition");
            else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
               cszOS += TEXT( "Enterprise x64 Edition");
            else cszOS += TEXT( "Standard x64 Edition");
         }

         else
         {
            if (osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER)
               cszOS += TEXT( "Compute Cluster Edition");
            else if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
               cszOS += TEXT( "Datacenter Edition");
            else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
               cszOS += TEXT( "Enterprise Edition");
            else if (osvi.wSuiteMask & VER_SUITE_BLADE)
               cszOS += TEXT( "Web Edition");
            else cszOS += TEXT( "Standard Edition");
         }
      }
   }

   if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
   {
      cszOS += TEXT("Windows XP ");
      if (osvi.wSuiteMask & VER_SUITE_PERSONAL)
         cszOS += TEXT( "Home Edition");
      else cszOS += TEXT( "Professional");
   }

   if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
   {
      cszOS += TEXT("Windows 2000 ");

      if (osvi.wProductType == VER_NT_WORKSTATION)
      {
         cszOS += TEXT( "Professional");
      }
      else
      {
         if (osvi.wSuiteMask & VER_SUITE_DATACENTER)
            cszOS += TEXT( "Datacenter Server");
         else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
            cszOS += TEXT( "Advanced Server");
         else cszOS += TEXT( "Server");
      }
   }

   // Include service pack (if any) and build number.

   if (_tcslen(osvi.szCSDVersion) > 0)
   {
      cszOS += TEXT(" ");
      cszOS += osvi.szCSDVersion;
   }

   CString cszTemp;
   cszTemp.Format(TEXT(" (build %d)"), osvi.dwBuildNumber);
   cszOS += cszTemp;

   if (osvi.dwMajorVersion >= 6)
   {
      if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
         cszOS += TEXT( ", 64-bit");
      else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
         cszOS += TEXT(", 32-bit");
   }

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
