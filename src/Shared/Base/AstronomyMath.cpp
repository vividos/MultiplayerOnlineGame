//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AstronomyMath.cpp Astronomy math functions
//

// includes
#include "StdAfx.h"
#include "AstronomyMath.hpp"
#include "Math.hpp"
#include <ulib/DateTime.hpp>

using namespace AstronomyMath;

/// calculates universal time from DateTime
double CalcUT(const DateTime& dt)
{
   unsigned int uiSeconds = dt.Hour() * 3600 + dt.Minute() * 60 + dt.Second();
   double UT = uiSeconds / 3600.0;
   return UT;
}

/// calculates day number (with time fractional part)
double DayNumber(const DateTime& dt)
{
   int iYear = dt.Year() - 2000;
   int d = 367 * iYear -
      int(7 * ( iYear + (dt.Month()+9)/12 ) / 4) +
      int(275*dt.Month()/9) + dt.Day() - 730530;

   double UT = CalcUT(dt);
   return d + UT/24.0;
}

/// calculates local siderial time from time and longitude of sun and local observer
double LocalSiderealTime(const DateTime& dt, double dSunLongitude, double dLocalLongitude)
{
   double UT = CalcUT(dt);

   double GMST0 = Rad2Deg(dSunLongitude) + 180.0;
   double GMST = GMST0 + UT * 15.0;
   double LST  = GMST + dLocalLongitude;

   return Deg2Rad(AngleInRange(LST));
}

/// converts right ascension (RA) and declination to azimuth and elevation (alt)
void RADecl2AzimuthElevation(double RA, double Decl, double dLocalLatitude, double LST,
                             double& az, double& alt)
{
   double HA = LST - RA;

   double x = cos(HA) * cos(Decl);
   double y = sin(HA) * cos(Decl);
   double z = sin(Decl);

   dLocalLatitude = Deg2Rad(dLocalLatitude);

   double xhor = x * sin(dLocalLatitude) - z * cos(dLocalLatitude);
   double yhor = y;
   double zhor = x * cos(dLocalLatitude) + z * sin(dLocalLatitude);

   az  = atan2(yhor, xhor) + Deg2Rad(180);
   alt = asin(zhor);

   az = Rad2Deg(az);
   alt = Rad2Deg(alt);
}

/// calculates longitude of the sun
double LongitudeSun(double d, double& r)
{
   // Orbital elements of the Sun
   double w = Deg2Rad(AngleInRange(282.9404 + 4.70935E-5 * d));
   double e = 0.016709 - 1.151E-9 * d;
   double M = Deg2Rad(AngleInRange(356.0470 + 0.9856002585 * d));

   double E = M + e * sin(M) * ( 1.0 + e * cos(M) );

   double xv = cos(E) - e;
   double yv = sqrt(1.0 - e*e) * sin(E);

   double v = atan2(yv, xv);
   r = sqrt(xv*xv + yv*yv);

   return v + w;
}

/// calculates obliquity of the ecliptic
double CalcEcliptic(double d)
{
   return Deg2Rad(AngleInRange(23.4393 - 3.563E-7 * d));
}

void AstronomyMath::SunPosition(const DateTime& dt,
                                double dLocalLatitude, double dLocalLongitude,
                                double& dAzimuth, double& dElevation)
{
   ATLASSERT(dLocalLatitude >= -90.0 && dLocalLatitude <= 90.0);
   ATLASSERT(dLocalLongitude >= 0.0 && dLocalLongitude < 360.0);

   double d = DayNumber(dt);


   double r;
   double lonsun = LongitudeSun(d, r);

   double xs = r * cos(lonsun);
   double ys = r * sin(lonsun);

   double ecl = CalcEcliptic(d);

   double xe = xs;
   double ye = ys * cos(ecl);
   double ze = ys * sin(ecl);

   double RA  = atan2(ye, xe);
   double Dec = atan2(ze, sqrt(xe*xe+ye*ye));

   double LST = LocalSiderealTime(dt, lonsun, dLocalLongitude);

   // RA / Dec -> azimuth / elevation at local point
   RADecl2AzimuthElevation(RA, Dec, dLocalLatitude, LST, dAzimuth, dElevation);
}

/// for an implementation to check against, see
/// http://www.stargazing.net/kepler/moon2.html
void AstronomyMath::MoonPosition(const DateTime& dt,
                                double dLocalLatitude, double dLocalLongitude,
                                double& dAzimuth, double& dElevation)
{
   ATLASSERT(dLocalLatitude >= -90.0 && dLocalLatitude <= 90.0);
   ATLASSERT(dLocalLongitude >= 0.0 && dLocalLongitude < 360.0);

   double d = DayNumber(dt);

   // Orbital elements of the Moon
   const double N = Deg2Rad(AngleInRange(125.1228 - 0.0529538083 * d));
   const double i = Deg2Rad(AngleInRange(5.1454));
   const double w = Deg2Rad(AngleInRange(318.0634 + 0.1643573223 * d));
   const double a = 60.2666; // Earth radii
   const double e = 0.054900;

   double M = Deg2Rad(AngleInRange(115.3654 + 13.0649929509 * d));

   double E = M + e * sin(M) * ( 1.0 + e * cos(M) );

   // note: E0/E1 iteration omitted

   double xv = a * (cos(E) - e);
   double yv = a * (sqrt(1.0 - e*e) * sin(E));

   double v = atan2(yv, xv);
   double r = sqrt(xv*xv + yv*yv);

   double xh = r * ( cos(N) * cos(v+w) - sin(N) * sin(v+w) * cos(i) );
   double yh = r * ( sin(N) * cos(v+w) + cos(N) * sin(v+w) * cos(i) );
   double zh = r * ( sin(v+w) * sin(i) );

   // calc moon's geocentric lon and lat
   //double lon = atan2(yh, xh);
   //double lat = atan2(zh, sqrt(xh*xh+yh*yh));

   // omitted: apply perturbations

   // find cartesian coordinates of the geocentric lunar position
   //double xg = r * cos(lon) * cos(lat);
   //double yg = r * sin(lon) * cos(lat);
   //double zg = r * sin(lat);

   // rotate to equatorial coords
   double ecl = CalcEcliptic(d);

   // note: shortcut; we use xh/yh/zh directly, as we didn't apply
   // perturbations; if implemented, replace with xh/yg/zg instead!
   double xe = xh;
   double ye = yh * cos(ecl) - zh * sin(ecl);
   double ze = yh * sin(ecl) + zh * cos(ecl);

   double RA  = atan2( ye, xe );
   double Dec = atan2( ze, sqrt(xe*xe+ye*ye) );

   // convert to LST
   double rSun;
   double lonsun = LongitudeSun(d, rSun);

   double LST = LocalSiderealTime(dt, lonsun, dLocalLongitude);

   // RA / Dec -> azimuth / elevation at local point
   RADecl2AzimuthElevation(RA, Dec, dLocalLatitude, LST, dAzimuth, dElevation);
}
