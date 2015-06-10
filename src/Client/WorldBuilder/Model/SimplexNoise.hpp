//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file SimplexNoise.hpp Simplex noise implementation
//
#pragma once

/// Simplex noise in 2D, 3D and 4D
/// \see http://staffwww.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf
class SimplexNoise
{
public:
   SimplexNoise();

   /// 2D simplex noise
   static double Noise(double xin, double yin);

   /// 3D simplex noise
   static double Noise(double xin, double yin, double zin);
};
