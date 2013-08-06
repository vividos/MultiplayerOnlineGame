//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Plane3d.hpp Plane class
//
#pragma once

// includes
#include "Base.hpp"
#include "Vector3d.hpp"

/// \brief plane in 3d space
class BASE_DECLSPEC Plane3d
{
public:
   /// default ctor
   Plane3d() throw()
      :m_dDist(0.0) {}

   /// constructs a plane from point on plane and normal; normal vector doesn't need to be normalized
   Plane3d(const Vector3d& vPos, const Vector3d& vNormal) throw();

   /// constructs a plane from 3 points on the plane
   Plane3d(const Vector3d& vPos1, const Vector3d& vPos2, const Vector3d& vPos3) throw();

   /// calculates signed distance of plane to point
   double Distance(const Vector3d& vPos) const throw();

private:
   Vector3d m_vNormal;  ///< normal vector
   double m_dDist;      ///< distance to origin
};
