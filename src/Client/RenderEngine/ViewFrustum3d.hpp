//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ViewFrustum3d.hpp 3D view frustum class
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include "Vector3.hpp"
#include "Plane3.hpp"

// forward references
class AABox;

/// \brief a camera view frustum in 3d space
/// \details implemented from:
/// \see http://www.lighthouse3d.com/tutorials/view-frustum-culling/
class RENDERENGINE_DECLSPEC ViewFrustum3d
{
public:
   /// result for intersect tests
   enum T_enIntersectResult
   {
      resultInside,     ///< completely inside view frustum
      resultOutside,    ///< completely outside
      resultIntersect,  ///< intersects with view frustum
   };

   /// ctor; takes position and parameters
   ViewFrustum3d(const Vector3d& vPos, double dAngle, double dAngleUp, double fov, double ratio, double nearDist, double farDist);

   /// checks if a given point is inside the frustum
   bool IsPointInside(const Vector3d& vPos) const throw();

   /// checks if a sphere is inside the frustum
   T_enIntersectResult IsSphereInside(const Vector3d& vPos, double dRadius) const throw();

   /// checks if a given axis-aligned box (e.g. a bounding box) is inside the frustum
   T_enIntersectResult IsBoxInside(const AABox& box) const throw();

   /// render frustum as lines
   static void RenderLines(const Vector3d& vPos, double dAngle, double dAngleUp, double fov, double ratio, double nearDist, double farDist);

private:
   /// internal names for m_aPlanes
   enum T_enPlaneNames
   {
      planeNear = 0,
      planeFar = 1,
      planeLeft = 2,
      planeRight = 3,
      planeTop = 4,
      planeBottom =5
   };

   /// all planes
   Plane3d m_aPlanes[6];
};
