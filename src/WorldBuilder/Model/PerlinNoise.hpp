//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file PerlinNoise.hpp Perlin noise implementation
//
#pragma once

// includes
#include <functional>

/// \see http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
/// \see http://mrl.nyu.edu/~perlin/noise/
/// \see http://www.noisemachine.com/talk1/
class PerlinNoise2D
{
public:
   /// noise function
   typedef std::function<double(int, int)> T_fnNoise2D;

   /// ctor
   PerlinNoise2D(unsigned int randomSeed, unsigned int numOctaves = 4, double persistence = 0.25) throw()
      :m_numOctaves(numOctaves),
       m_persistence(persistence),
       m_randomSeed(2 + randomSeed * randomSeed),
       m_fnNoise2D(&PerlinNoise2D::Noise2D)
   {
   }

   void SetNoiseFunction(T_fnNoise2D fnNoise2D) throw() { m_fnNoise2D = fnNoise2D; }

   double Get(double x, double y) const throw();

private:
   static double Noise2D(int x, int y) throw();
   double SmoothNoise2D(int x, int y) const throw();
   double InterpolatedNoise(double x, double y) const throw();

private:
   unsigned int m_numOctaves;
   double m_persistence;
   unsigned int m_randomSeed;

   T_fnNoise2D m_fnNoise2D;
};

#include <boost/array.hpp>

/// \see http://mrl.nyu.edu/~perlin/noise/
/// http://webstaff.itn.liu.se/~stegu/TNM022-2005/perlinnoiselinks/perlin-noise-math-faq.html
class ImprovedNoise
{
public:
   ImprovedNoise() throw();

   /// returns values in range [-1.0, 1.0]
   double noise(double x, double y, double z) const throw();

private:
   std::array<int, 512> p;
};
