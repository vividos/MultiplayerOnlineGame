//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file TestVector3d.cpp Unit tests for class Vector3d
//

// includes
#include "stdafx.h"
#include "Vector3d.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

/// tests Vector3d class
TEST_CLASS(TestVector3d)
{
   /// tests default ctor
   TEST_METHOD(TestDefaultCtor)
   {
      Vector3d v1, v2;
      Assert::AreEqual(v1.Length(), 0.0, 0.1);
      Assert::AreEqual(v1.Length(), v2.Length(), 0.1);
   }
};

} // namespace UnitTest
