//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file TestService.cpp TestService main function
//

// includes
#include "stdafx.h"
#include "TestService.hpp"
#include "ServiceRunner.hpp"
#include "TestServiceControlManager.hpp"
#include "AttachedConsole.hpp"

IMPLEMENT_SERVICE(_T("testservice"), TestService)

/// TestService main function
int APIENTRY _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
   // check if we were started as service; when not, the Run() call throws
   try
   {
      Win32::ServiceRunner runner;
      runner.AddService<TestService>();
      runner.Run();
   }
   catch (SystemException& ex)
   {
      // we were started normally; run unit tests
      if (ex.Win32Error() == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
      {
         Win32::AttachedConsole con;

         _tprintf(_T("TestService - Unit tests for MultiplayerOnlineGame\n\n"));

         UnitTest::TestServiceControlManager test;
         return test.RunTests() ? 0 : 1;
      }
   }

   return 0;
}
