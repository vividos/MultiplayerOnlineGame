//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Plane3.hpp Plane class
//
#pragma once

// includes
#include "Base.hpp"
#include "Vector3.hpp"

/// \brief plane in 3d space
class BASE_DECLSPEC Plane3d
{
public:
   /// default ctor
   Plane3d()
      :m_dDist(0.0) {}

   /// constructs a plane from point on plane and normal; normal vector doesn't need to be normalized
   Plane3d(const Vector3d& vPos, const Vector3d& vNormal);

   /// constructs a plane from 3 points on the plane
   Plane3d(const Vector3d& vPos1, const Vector3d& vPos2, const Vector3d& vPos3);

   /// calculates signed distance of plane to point
   double Distance(const Vector3d& vPos) const;

private:
   Vector3d m_vNormal;  ///< normal vector
   double m_dDist;      ///< distance to origin
};
