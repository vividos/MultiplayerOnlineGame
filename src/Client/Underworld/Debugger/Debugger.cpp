//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Debugger.cpp Debugger class and entry point
//

// includes
#include "stdafx.h"
#include "Debugger.hpp"
#include "Resource.h"
#include "MainFrame.hpp"

// globals and defines
CAppModule _Module;
HINSTANCE g_hInstance = 0;

// methods

Debugger::Debugger(DebugClient& client)
:m_client(client)
{
   // init COM
   HRESULT hRes = ::CoInitialize(NULL);
   ATLASSERT(SUCCEEDED(hRes));

   // this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
   ::DefWindowProc(NULL, 0, 0, 0L);

   AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);

   DWORD dwMajor = 0, dwMinor = 0;
   ::AtlGetCommCtrlVersion(&dwMajor, &dwMinor);
   ATLTRACE(_T("using common controls version %u.%u\n"), dwMajor, dwMinor);

   hRes = _Module.Init(NULL, g_hInstance/*, &LIBID_ATLLib*/);
   ATLASSERT(SUCCEEDED(hRes));
}

Debugger::~Debugger()
{
   _Module.Term();
   ::CoUninitialize();
}

int Debugger::Run()
{
   MainFrame wndMain(m_client);

   CMessageLoop theLoop;
   _Module.AddMessageLoop(&theLoop);

   if (wndMain.CreateEx() == NULL)
   {
      ATLTRACE(_T("Main window creation failed!\n"));
      return 0;
   }

   wndMain.ShowWindow(SW_SHOWDEFAULT);

   int nRet = theLoop.Run();

   _Module.RemoveMessageLoop();

   return nRet;
}

/// debugger start function
extern "C"
__declspec(dllexport)
void Debug(DebugClient* pDebugClient)
{
   ATLASSERT(pDebugClient != nullptr);

   Debugger d(*pDebugClient);
   d.Run();
}

/// DLL Entry Point
extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
   if (dwReason == DLL_PROCESS_ATTACH)
   {
      // remember instance handle
      g_hInstance = hInstance;
      DisableThreadLibraryCalls(hInstance);
   }

   return TRUE;
}
