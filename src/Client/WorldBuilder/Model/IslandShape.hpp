//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file IslandShape.hpp Island shape classes
//
#pragma once

// include
#include "Vector2.hpp"

/// island shape base class
class IslandShape
{
public:
   /// dtor
   virtual ~IslandShape() {}

   /// determines if point is on the island; x and y are in range [-0.5;0.5]
   virtual bool IsInIsland(const Vector2d& point) const = 0;
};

/// The square shape fills the entire space with land
class SquareIslandShape: public IslandShape
{
public:
   /// dtor
   virtual ~SquareIslandShape() {}

   /// determines if point is on the island
   virtual bool IsInIsland(const Vector2d& /*point*/) const override
   {
      return true;
   }
};

#include <boost/random.hpp>

/// The radial island radius is based on overlapping sine waves
class RadialIslandShape: public IslandShape
{
public:
   /// ctor
   RadialIslandShape(unsigned int seed)
      :m_rng(seed),
       ISLAND_FACTOR(1.07) // 1.0 means no small islands; 2.0 leads to a lot
   {
      boost::uniform_int<> distInt(1, 6);
      boost::variate_generator<boost::mt19937&, boost::uniform_int<> > dieInt(m_rng, distInt);
      m_bumps = dieInt();

      boost::uniform_real<> dist2pi(0.0, g_dPi);
      boost::variate_generator<boost::mt19937&, boost::uniform_real<> > die2pi(m_rng, dist2pi);

      m_startAngle = die2pi();
      m_dipAngle = die2pi();

      boost::uniform_real<> distWidth(0.2, 0.7);
      boost::variate_generator<boost::mt19937&, boost::uniform_real<> > dieWidth(m_rng, distWidth);
      m_dipWidth = dieWidth();
   }

   /// dtor
   virtual ~RadialIslandShape() {}

   virtual bool IsInIsland(const Vector2d& point) const override
   {
      double angle = Deg2Rad(point.PolarAngle());
      double length = 0.5 * (std::max(fabs(point.X()), fabs(point.Y())) + point.Length());

      double r1 = 0.5 + 0.40 * sin(m_startAngle + m_bumps*angle + cos((m_bumps+3)*angle));
      double r2 = 0.7 - 0.20 * sin(m_startAngle + m_bumps*angle - sin((m_bumps+2)*angle));

      if (fabs(angle - m_dipAngle) < m_dipWidth ||
          fabs(angle - m_dipAngle + 2*g_dPi) < m_dipWidth ||
          fabs(angle - m_dipAngle - 2*g_dPi) < m_dipWidth)
      {
         r1 = r2 = 0.2;
      }

      return length < r1 || (length > r1*ISLAND_FACTOR && length < r2);
   }

private:
   const double ISLAND_FACTOR; ///< 1.0 means no small islands; 2.0 leads to a lot

   boost::mt19937 m_rng;
   unsigned int m_bumps;
   double m_startAngle;
   double m_dipAngle;
   double m_dipWidth;
};

#include "SimplexNoise.hpp"
#include "PerlinNoise.hpp"

/// The Perlin-based island combines perlin noise with the radius
class PerlinIslandShape: public IslandShape
{
public:
   /// ctor
   PerlinIslandShape(unsigned int seed)
      :m_seed(seed)
   {
   }

   /// dtor
   virtual ~PerlinIslandShape() {}

   virtual bool IsInIsland(const Vector2d& point) const override
   {
      // convert x and y from [-0.5; 0.5] to [0.0; 4.0]
      LinearTransform<double> ltxy(-0.5, 0.5, 0.0, 4.0);
      // convert c from [-1.0; 1.0] to [0.0; 1.0]
      LinearTransform<double> ltc(-1.0, 1.0, 0.0, 1.0);

      double x = ltxy(point.X());
      double y = ltxy(point.Y());
      double z = m_seed * 10.0;

      double c = m_perlin.noise(x, y, z);

      // c is in [-1.0;1.0]
      c = ltc(c);

      ATLASSERT(c >= 0.0 && c < 1.0);
      if (c > 1.0)
         c = 1.0;
      if (c < 0.0)
         c = 0.0;

      return c > (0.3 + 0.3*point.Length()*point.Length());
   }

private:
   unsigned int m_seed;
   ImprovedNoise m_perlin;
};
