//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestAstronomyMath.cpp Unit tests for AstronomyMath functions
//

// includes
#include "stdafx.h"
#include <ulib/DateTime.hpp>
#include "AstronomyMath.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
const double c_dLocalLatitude = 48.148221;
const double c_dLocalLongitude = 11.57959;

/// tests AstronomyMath functions
TEST_CLASS(TestAstronomyMath)
{

   /// tests function SunPosition()
   TEST_METHOD(TestSunPosition)
   {
      DateTime dt(2011, 9, 29, 19, 37, 0);

      double dAzimuth = 0.0, dElevation = 0.0;

      AstronomyMath::SunPosition(dt,
         c_dLocalLatitude, c_dLocalLongitude,
         dAzimuth, dElevation);

      Assert::AreEqual(dAzimuth, 299.90, 1.0);
      Assert::AreEqual(dElevation, -19.57, 1.0);
   }

   /// tests function MoonPosition()
   TEST_METHOD(TestMoonPosition)
   {
      DateTime dt(2011, 9, 29, 19, 37, 0);

      double dAzimuth = 0.0, dElevation = 0.0;

      AstronomyMath::MoonPosition(dt,
         c_dLocalLatitude, c_dLocalLongitude,
         dAzimuth, dElevation);

      Assert::AreEqual(dAzimuth, 293.48, 1.0);
      Assert::AreEqual(dElevation, -13.46, 1.0);
   }
};

} // namespace UnitTest
