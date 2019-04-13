//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ViewFrustum3d.cpp 2D view frustum class
//

// includes
#include "StdAfx.h"
#include "ViewFrustum2d.hpp"
#include "Math.hpp"

ViewFrustum2d::ViewFrustum2d(double xpos, double ypos,
   double angle, double fov, double farplane)
{
   m_points[0] = Vector2d(xpos - 3.0*cos(Deg2Rad(angle)), ypos - 3.0*sin(Deg2Rad(angle)));

   // view vector
   double vx = farplane * cos(Deg2Rad(angle)) + xpos;
   double vy = farplane * sin(Deg2Rad(angle)) + ypos;

   // vector to second point
   double len = farplane * tan(Deg2Rad(fov/2.0));
   double wx = len * cos(Deg2Rad(angle+90.0));
   double wy = len * sin(Deg2Rad(angle+90.0));

   m_points[1] = Vector2d(vx + wx, vy + wy);
   m_points[2] = Vector2d(vx - wx, vy - wy);
}

/// Determines if the given point is inside the view frustum.
/// This function uses the barycentric coordinates of the view frustum
/// triangle to determine if a point is in the frustum.
/// \see http://research.microsoft.com/~hollasch/cgindex/math/barycentric.html
/// \param xpos x position
/// \param ypos y position
bool ViewFrustum2d::IsInside(double xpos, double ypos) const
{
   double b0 =  (m_points[1].X() - m_points[0].X()) * (m_points[2].Y() - m_points[0].Y()) - (m_points[2].X() - m_points[0].X()) * (m_points[1].Y() - m_points[0].Y());
   double b1 = ((m_points[1].X() - xpos) * (m_points[2].Y() - ypos) - (m_points[2].X() - xpos) * (m_points[1].Y() - ypos)) / b0;
   double b2 = ((m_points[2].X() - xpos) * (m_points[0].Y() - ypos) - (m_points[0].X() - xpos) * (m_points[2].Y() - ypos)) / b0;
   double b3 = ((m_points[0].X() - xpos) * (m_points[1].Y() - ypos) - (m_points[1].X() - xpos) * (m_points[0].Y() - ypos)) / b0;

   return b1 > 0.0 && b2 > 0.0 && b3 > 0.0;
}
