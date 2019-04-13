//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ViewFrustum2d.hpp 2D view frustum class
//
#pragma once

// includes
#include "Vector2.hpp"

/// a 2D camera view frustum
class RENDERENGINE_DECLSPEC ViewFrustum2d
{
public:
   /// ctor; takes position, camera angle, field of view and far plane distance
   ViewFrustum2d(double xpos, double ypos,
      double angle, double fov, double farplane);

   /// checks if a given point is inside the frustum
   bool IsInside(double xpos, double ypos) const;

private:
   /// frustum points
   Vector2d m_points[3];
};
