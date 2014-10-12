//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestUpdateManager.cpp Unit tests for class UpdateManager
//

// includes
#include "stdafx.h"
#include "UpdateManager.hpp"
#include "TestSessionManager.hpp"
#include "Mobile.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests class UpdateManager
   TEST_CLASS(TestUpdateManager)
   {
      /// tests default ctor
      TEST_METHOD(TestDefaultCtor)
      {
         boost::asio::io_service ioService;
         TestSessionManager sm(ioService);
         UpdateManager um(sm);
      }

      /// tests ShareAddObject()
      TEST_METHOD(Test2)
      {
         boost::asio::io_service ioService;
         TestSessionManager sm(ioService);
         UpdateManager um(sm);

         MobilePtr spMobile1(new Mobile(ObjectId::New()));
         MobilePtr spMobile2(new Mobile(ObjectId::New()));

         um.ShareAddObject(spMobile1);

         um.ShareAddObject(spMobile2);
      }
   };

} // namespace UnitTest
