//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Plane3.cpp Plane class
//

// includes
#include "StdAfx.h"
#include "Plane3.hpp"

Plane3d::Plane3d(const Vector3d& vPos, const Vector3d& vNormal)
:m_vNormal(vNormal)
{
   m_vNormal.Normalize();
   m_dDist = -m_vNormal.Dot(vPos);
}

Plane3d::Plane3d(const Vector3d& vPos1, const Vector3d& vPos2, const Vector3d& vPos3)
{
   // calculate normal
   Vector3d vDir1 = vPos2 - vPos1;
   Vector3d vDir2 = vPos3 - vPos1;
   m_vNormal.Cross(vDir1, vDir2);
   m_vNormal.Normalize();

   m_dDist = -m_vNormal.Dot(vPos1);
}

/// \details when signed distance is positive, point lies on the same side
/// as the normal is pointing towards; when it is negative, point lies on the
/// opposite side of the plane.
/// \see http://www.lighthouse3d.com/cg-topics/maths/plane/
double Plane3d::Distance(const Vector3d& vPos) const
{
   return m_vNormal.Dot(vPos) + m_dDist;
}
