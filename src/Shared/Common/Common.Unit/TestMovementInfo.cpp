//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file TestMovementInfo.cpp Unit tests for class MovementInfo
//

// includes
#include "stdafx.h"
#include "MovementInfo.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   /// tests class MovementInfo
   TEST_CLASS(TestMovementInfo)
   {
      /// tests default ctor
      TEST_METHOD(TestDefaultCtor)
      {
         MovementInfo m;
      }
   };

} // namespace UnitTest
