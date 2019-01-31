//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestServiceControlManager.hpp Unit tests for class ServiceControlManager
//

// includes
#include "stdafx.h"
#include <ulib/Exception.hpp>

namespace UnitTest
{

/// tests class Win32::ServiceControlManager
/// \note all tests need administrative privileges to control services
class TestServiceControlManager
{
public:
   bool RunTests();

private:
   void TestDefaultCtor();
   void TestRegisterUnregister();
   void TestRegisterOpenUnregister();
   void TestStartStop();
   void TestStartRestartStop();
   void TestStartUnregister();
   void TestStartRegister();
   void TestOpenRestart();
   void TestStopNotRunning();

   void CleanupUnregisterService();
};

} // namespace UnitTest
