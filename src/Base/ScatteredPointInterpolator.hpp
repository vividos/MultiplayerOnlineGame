//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ScatteredPointInterpolator.hpp Scattered point interpolator
//
#pragma once

// includes
#include "Base.hpp"
#include "Vector3d.hpp"
#include "Vector2d.hpp"
#include <vector>

/// \brief scattered point interpolator
/// \details uses Radial Basis Function to interpolate
class BASE_DECLSPEC ScatteredPointInterpolator
{
public:
   /// ctor
   ScatteredPointInterpolator(double dSigma, const std::vector<Vector3d>& vecPoints);

   /// inits interpolator
   void Init();

   /// returns height value for 2d height map position
   double Height(const Vector2d& vPos);

private:
   /// gaussian normal distribution
   double Gaussian(double x);

private:
   /// sigma value of gaussian bell curve
   double m_dSigma;

   /// scattered reference points
   std::vector<Vector3d> m_vecPoints;

   /// weights for all reference points
   std::vector<double> m_vecWeights;
};
