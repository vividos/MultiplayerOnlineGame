//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Audio/AudioPlayer/App.cpp Audio player application
//

// includes
#include "stdafx.h"
#include "App.hpp"
#include "resource.h"
#include "AudioPlayerDlg.hpp"
#include <crtdbg.h>
#include <ulib/log/Appender.hpp>
#include <ulib/log/Layout.hpp>

 /// WTL app model
CAppModule _Module;

App::App(HINSTANCE hInstance)
{
#ifdef _DEBUG
   // turn on leak-checking
   int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
   _CrtSetDbgFlag(flag | _CRTDBG_LEAK_CHECK_DF);
#endif

   // note: Apartment Threading needed for Canon ED-SDK
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

int App::Run(LPCTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
   AudioPlayerDlg dlg;
   return dlg.DoModal();
}

void App::SetupLogging()
{
   Log::AppenderPtr spTraceAppender(new Log::OutputDebugStringAppender);

   Log::LayoutPtr spTraceLayout(new Log::PatternLayout(_T("%F(%L): log [%p] %m")));
   spTraceAppender->Layout(spTraceLayout);

   Log::Logger::GetRootLogger()->AddAppender(spTraceAppender);
}

/// main function
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
   App::SetupLogging();

   App app(hInstance);
   return app.Run(lpstrCmdLine, nCmdShow);
}
