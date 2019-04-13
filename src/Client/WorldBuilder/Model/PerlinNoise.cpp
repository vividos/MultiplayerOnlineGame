//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file PerlinNoise.cpp Perlin noise implementation
//

// include
#include "StdAfx.h"
#include "PerlinNoise.hpp"
#include "Math.hpp"

double CosineInterpolate(double d0, double d1, double delta)
{
   ATLASSERT(delta >= 0.0 && delta <= 1.0);

   double f = (1.0 - cos(delta * g_dPi)) * 0.5;

   return Interpolate(d0, d1, f);
}


double Noise1D(int x)
{
   x = (x<<13) ^ x;
   int xx = (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff;
   return 1.0 - xx / 1073741824.0;
}

double PerlinNoise2D::Noise2D(int x, int y)
{
   int n = x + y * 57;
   return Noise1D(n);
}

double PerlinNoise2D::SmoothNoise2D(int x, int y) const
{
   double corners = ( m_fnNoise2D(x-1, y-1) + m_fnNoise2D(x+1, y-1) + m_fnNoise2D(x-1, y+1) + m_fnNoise2D(x+1, y+1) ) / 16.0;
   double sides   = ( m_fnNoise2D(x-1, y)   + m_fnNoise2D(x+1, y)   + m_fnNoise2D(x, y-1)   + m_fnNoise2D(x, y+1) ) /  8.0;
   double center  =  m_fnNoise2D(x, y) / 4;

   return corners + sides + center;
}

double PerlinNoise2D::InterpolatedNoise(double x, double y) const
{
   int intx = int(floor(x));
   double fractx = x - intx;

   int inty = int(floor(y));
   double fracty = y - inty;

   double v1 = SmoothNoise2D(intx,     inty);
   double v2 = SmoothNoise2D(intx + 1, inty);
   double v3 = SmoothNoise2D(intx,     inty + 1);
   double v4 = SmoothNoise2D(intx + 1, inty + 1);

   double i1 = CosineInterpolate(v1, v2, fractx);
   double i2 = CosineInterpolate(v3, v4, fractx);

   return Interpolate(i1, i2, fracty);
}

double PerlinNoise2D::Get(double x, double y) const
{
   double total = 0.0;

   for (unsigned int i=0; i<m_numOctaves - 1; i++)
   {
      double frequency = 1 << i; // 2^i
      //double frequency = 32.0;
      double amplitude = std::pow(m_persistence, double(i));  /// p^i

      total = total + InterpolatedNoise(
         x * frequency /*+ m_randomSeed*/,
         y * frequency /*+ m_randomSeed*/) * amplitude;
   }

   return total;
}




inline double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }

inline double lerp(double t, double a, double b) { return a + t * (b - a); }

inline double grad(int hash, double x, double y, double z)
{
   int h = hash & 15;
   double u = h<8 ? x : y,
          v = h<4 ? y : h==12 || h==14 ? x : z;
   return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

double ImprovedNoise::noise(double x, double y, double z) const
{
   int intX = int(floor(x)) & 255,
       intY = int(floor(y)) & 255,
       intZ = int(floor(z)) & 255;
   x -= floor(x);
   y -= floor(y);
   z -= floor(z);
   double u = fade(x),
         v = fade(y),
         w = fade(z);

   int A = p[intX  ]+intY, AA = p[A]+intZ, AB = p[A+1]+intZ,
       B = p[intX+1]+intY, BA = p[B]+intZ, BB = p[B+1]+intZ;

   return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),
                                  grad(p[BA  ], x-1, y  , z   )),
                          lerp(u, grad(p[AB  ], x  , y-1, z   ),
                                  grad(p[BB  ], x-1, y-1, z   ))),
                  lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),
                                  grad(p[BA+1], x-1, y  , z-1 )),
                          lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
                                  grad(p[BB+1], x-1, y-1, z-1 ))));
}

static const int permutation[256] =
{
   151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

ImprovedNoise::ImprovedNoise()
{
   const size_t uiLen = sizeof(permutation)/sizeof(*permutation);
   ATLASSERT(uiLen * 2 == p.size());

   memcpy(&p[0], permutation, uiLen*sizeof(*permutation));
   memcpy(&p[0]+uiLen, permutation, uiLen*sizeof(*permutation));
}
