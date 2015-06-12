//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AstronomyMath.hpp Astronomy math functions
//
#pragma once

// includes
#include "Base.hpp"

// forward references
class DateTime;

/// \brief astronomy math functions
/// \see http://stjarnhimlen.se/comp/ppcomp.html
namespace AstronomyMath
{
   /// calculates sun position from current date/time and location position
   void BASE_DECLSPEC SunPosition(const DateTime& dt,
      double dLocalLatitude, double dLocalLongitude,
      double& dAzimuth, double& dElevation);

   /// calculates moon position from current date/time and location position
   void BASE_DECLSPEC MoonPosition(const DateTime& dt,
      double dLocalLatitude, double dLocalLongitude,
      double& dAzimuth, double& dElevation);

} // AstronomyMath
