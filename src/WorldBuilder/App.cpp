//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file App.cpp Application
//

// includes
#include "stdafx.h"
#include "App.hpp"
#include "resource.h"
#include "MainFrame.hpp"
#include <crtdbg.h>

 /// WTL app model
CAppModule _Module;

App::App(HINSTANCE hInstance)
{
#ifdef _DEBUG
   // turn on leak-checking
   int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
   _CrtSetDbgFlag(flag | _CRTDBG_LEAK_CHECK_DF);
#endif

   HRESULT hRes = ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
   ATLASSERT(SUCCEEDED(hRes));

   // this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
   ::DefWindowProc(NULL, 0, 0, 0L);

   AtlInitCommonControls(ICC_WIN95_CLASSES);

   hRes = _Module.Init(NULL, hInstance);
   ATLASSERT(SUCCEEDED(hRes));
}

App::~App()
{
   _Module.Term();
   ::CoUninitialize();
}

int App::Run(LPCTSTR /*lpstrCmdLine*/, int nCmdShow)
{
   CMessageLoop theLoop;
   _Module.AddMessageLoop(&theLoop);

   MainFrame wndMain;

   if(wndMain.CreateEx() == nullptr)
   {
      ATLTRACE(_T("Main window creation failed!\n"));
      return 0;
   }

   wndMain.ShowWindow(nCmdShow);

   int nRet = theLoop.Run();

   _Module.RemoveMessageLoop();
   return nRet;
}
