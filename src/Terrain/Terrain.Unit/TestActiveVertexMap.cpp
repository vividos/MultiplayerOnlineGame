//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestActiveVertexMap.cpp Unit tests for ActiveVertexMap class
//

// includes
#include "stdafx.h"
#include "Reduce/ActiveVertexMap.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using Terrain::Reduce::ActiveVertexMap;

namespace UnitTest
{

/// tests ActiveVertexMap class
TEST_CLASS(TestActiveVertexMap)
{
   /// tests default ctor
   TEST_METHOD(TestDefaultCtor)
   {
      ActiveVertexMap m1(64), m2(16);
   }

   /// tests IsInMap() method
   TEST_METHOD(TestIsInMap)
   {
      ActiveVertexMap m(8);

      Assert::IsTrue(true == m.IsInMap(0,0));
      Assert::IsTrue(true == m.IsInMap(2,3));
      Assert::IsTrue(true == m.IsInMap(5,7));

      Assert::IsTrue(false == m.IsInMap(0,9));
      Assert::IsTrue(false == m.IsInMap(9,0));
      Assert::IsTrue(false == m.IsInMap(64,42));
   }

   /// tests IsActive() method
   TEST_METHOD(TestIsActive)
   {
      ActiveVertexMap m(8);

      Assert::IsTrue(false == m.IsActive(2,3));
      Assert::IsTrue(true == m.IsActive(0,9));
      Assert::IsTrue(true == m.IsActive(9,0));
   }

   /// tests SetActive() method
   TEST_METHOD(TestSetActive)
   {
      ActiveVertexMap m(8);

      m.SetActive(2,3);

      Assert::IsTrue(true == m.IsActive(2,3));
      Assert::IsTrue(false == m.IsActive(5,7));
   }

   /// tests copy ctor
   TEST_METHOD(TestCopyCtor)
   {
      ActiveVertexMap m(8);
      m.SetActive(2,3);
      Assert::IsTrue(true == m.IsActive(2,3));

      ActiveVertexMap m2(m); // copy

      Assert::IsTrue(true == m2.IsActive(2,3));
      Assert::IsTrue(false == m2.IsActive(5,7));
   }
};

} // namespace UnitTest
