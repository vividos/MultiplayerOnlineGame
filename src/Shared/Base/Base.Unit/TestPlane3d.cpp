//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestPlane3d.cpp Unit tests for class Plane3d
//

// includes
#include "stdafx.h"
#include "Plane3.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

/// tests class Plane3d
TEST_CLASS(TestPlane3d)
{
   /// tests Plane3d, part 1
   TEST_METHOD(TestParallelPlaneDistance)
   {
      Plane3d p(Vector3d(), Vector3d(0.0, 0.0, 1.0));

      double dDist = p.Distance(Vector3d(0.0, 0.0, 0.0));
      Assert::AreEqual(dDist, 0.0, 0.1);

      dDist = p.Distance(Vector3d(0.0, 0.0, 5.0));
      Assert::AreEqual(dDist, 5.0, 0.1);

      dDist = p.Distance(Vector3d(0.0, 0.0, -5.0));
      Assert::AreEqual(dDist, -5.0, 0.1);
   }

   /// tests Plane3d, part 2
   TEST_METHOD(TestIntersectingPlaneDistance)
   {
      Plane3d p(Vector3d(1.0, 1.0, 1.0), Vector3d(1.0, 1.0, 1.0));

      double dDist = p.Distance(Vector3d(0.0, 0.0, 0.0));
      Assert::IsTrue(dDist < 0.0);

      dDist = p.Distance(Vector3d(2.0, 2.0, 2.0));
      Assert::IsTrue(dDist > 0.0);
   }
};

} // namespace UnitTest
