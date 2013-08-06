//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file TestByteStream.cpp Unit tests for class ByteStream
//

// includes
#include "stdafx.h"
#include "ByteStream.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

/// tests class ByteStream
TEST_CLASS(TestByteStream)
{
   /// tests Read8() method
   TEST_METHOD(TestRead8)
   {
      std::vector<unsigned char> vecData(1, 42);
      VectorStream s(vecData);
      Assert::IsTrue(42 == s.Read8());
   }

   /// tests Read16() method
   TEST_METHOD(TestRead16)
   {
      std::vector<unsigned char> vecData(2, 42);
      VectorStream s(vecData);
      Assert::IsTrue(0x2a2a == s.Read16());
   }

   /// tests Read32() method
   TEST_METHOD(TestRead32)
   {
      std::vector<unsigned char> vecData(4, 42);
      VectorStream s(vecData);
      Assert::IsTrue(0x2a2a2a2a == s.Read32());
   }
};

} // namespace UnitTest
