//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file BSpline.hpp B-Spline classes
//
#pragma once

// includes
#include <vector>
#include "Vector3.hpp"

/// \brief calculates a 1d rational b-spline
/// \details The code to calculate spline curves was adapted from the example code
/// of the book "An Introduction to NURBS", by David F. Rogers.
class BSpline: public boost::noncopyable
{
public:
   /// ctor
   BSpline(unsigned int uiOrder = 3);

   /// generates the b-spline curve
   void Generate(const std::vector<float>& vecPoints);

   /// returns the value at a specific point; 0.0 <= at <= 1.0
   float Get(float at) const;

private:
   /// calculates b-spline base vector
   void CalcBase(float at);

private:
   int m_uiOrder; ///< order of the bspline equation

   /// current base vector
   std::vector<float> m_vecBase;

   /// knot vectors
   std::vector<int> m_vecKnot;

   /// temporary vector
   std::vector<float> m_vecTemp;

   /// control points
   std::vector<float> m_vecPoints;

   /// number of control points
   size_t m_uiNumControlPoints;
};

/// calculates a 3d b-spline
class BSpline3D: public boost::noncopyable
{
public:
   /// ctor
   BSpline3D(unsigned int uiOrder = 3)
      :m_bx(uiOrder),
       m_by(uiOrder),
       m_bz(uiOrder)
   {
   }

   /// generates the 3d b-spline
   void Generate(const std::vector<Vector3d>& vecPoints);

   /// returns bspline vector
   Vector3d Get(float at) const
   {
      return Vector3d(
         m_bx.Get(at),
         m_by.Get(at),
         m_bz.Get(at));
   }

private:
   /// b-splines for every coordinate value
   BSpline m_bx, m_by, m_bz;
};
