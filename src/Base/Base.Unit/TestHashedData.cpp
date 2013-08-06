//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file TestHashedData.cpp Unit tests for class HashedData
//

// includes
#include "stdafx.h"
#include "HashedData.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

/// tests HashedData class
TEST_CLASS(TestHashedData)
{
   /// tests default ctor
   TEST_METHOD(TestDefaultCtor)
   {
      std::vector<unsigned char> vecEmpty;
      HashedData hd(vecEmpty);

      std::vector<unsigned char> vecData = hd.Get();

      Assert::AreEqual(vecData.size(), 32);

      Assert::AreEqual(vecData[0], 227);
      Assert::AreEqual(vecData[31], 85);
   }

   /// tests sha-256 value of 42
   TEST_METHOD(TestHashValue42)
   {
      std::vector<unsigned char> vecData(1, 42);
      HashedData hd(vecData);

      vecData = hd.Get();

      Assert::AreEqual(vecData.size(), 32);

      Assert::AreEqual(vecData[0], 104);
      Assert::AreEqual(vecData[31], 193);
   }
};

} // namespace UnitTest
