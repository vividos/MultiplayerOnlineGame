//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ViewFrustum3d.cpp 3D view frustum class
//

// includes
#include "StdAfx.h"
#include "ViewFrustum3d.hpp"
#include "Vector3d.hpp"
#include "AABox.hpp"
#include "OpenGL.hpp"

/// calculates view frustum coordinates from given parameters
struct FrustumCoordinates
{
   /// ctor
   FrustumCoordinates(const Vector3d& vPos, double dAngleDir, double dAngleUp,
                      double fov, double ratio, double nearDist, double farDist)
      :d(0.0, 0.0, 1.0) // z
   {
      d.RotateX(dAngleUp);
      d.RotateY(dAngleDir);
      d.Normalize();

      Vector3d p = vPos;

      Vector3d up(0.0, 1.0, 0.0);
      up.RotateX(dAngleUp);
      up.RotateY(dAngleDir);
      up.Normalize();

      Vector3d right(1.0, 0.0, 0.0);
      right.RotateX(dAngleUp);
      right.RotateY(dAngleDir);
      right.Normalize();

      double Hnear = 2 * tan(Deg2Rad(fov / 2.0)) * nearDist;
      double Wnear = Hnear * ratio;

      double Hfar = 2 * tan(Deg2Rad(fov / 2.0)) * farDist;
      double Wfar = Hfar * ratio;

      Vector3d fc = p + d * farDist;

      ftl = fc + (up * (Hfar/2)) - (right * (Wfar/2));
      ftr = fc + (up * (Hfar/2)) + (right * (Wfar/2));
      fbl = fc - (up * (Hfar/2)) - (right * (Wfar/2));
      fbr = fc - (up * (Hfar/2)) + (right * (Wfar/2));

      Vector3d nc = p + d * nearDist;

      ntl = nc + (up * (Hnear/2)) - (right * (Wnear/2));
      ntr = nc + (up * (Hnear/2)) + (right * (Wnear/2));
      nbl = nc - (up * (Hnear/2)) - (right * (Wnear/2));
      nbr = nc - (up * (Hnear/2)) + (right * (Wnear/2));
   }

   Vector3d d;   ///< view direction

   Vector3d ftl; ///< far top left
   Vector3d ftr; ///< far top right
   Vector3d fbl; ///< far bottom left
   Vector3d fbr; ///< far bottom right

   Vector3d ntl; ///< near top left
   Vector3d ntr; ///< near top right
   Vector3d nbl; ///< near bottom left
   Vector3d nbr; ///< near bottom right
};

/// \see http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
ViewFrustum3d::ViewFrustum3d(const Vector3d& vPos, double dAngleDir, double dAngleUp,
                             double fov, double ratio, double nearDist, double farDist)
{
   FrustumCoordinates fc(vPos, dAngleDir, dAngleUp, fov, ratio, nearDist, farDist);

   m_aPlanes[planeNear] = Plane3d(fc.ntl, fc.d);
   m_aPlanes[planeFar] =  Plane3d(fc.ftl, -fc.d);
   m_aPlanes[planeLeft] =   Plane3d(fc.ntl, fc.ftl, fc.fbl);
   m_aPlanes[planeRight] =  Plane3d(fc.ntr, fc.nbr, fc.ftr);
   m_aPlanes[planeTop] =    Plane3d(fc.ntl, fc.ntr, fc.ftr);
   m_aPlanes[planeBottom] = Plane3d(fc.nbr, fc.nbl, fc.fbl);
}

/// \see http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-testing-points-and-spheres/
bool ViewFrustum3d::IsPointInside(const Vector3d& vPos) const throw()
{
   for (unsigned int i=0; i<6; i++)
      if (m_aPlanes[i].Distance(vPos) < 0)
         return false;

   return true;
}

ViewFrustum3d::T_enIntersectResult ViewFrustum3d::IsSphereInside(const Vector3d& vPos, double dRadius) const throw()
{
   T_enIntersectResult result = resultInside;
   for (unsigned int i=0; i<6; i++)
   {
      double dDist = m_aPlanes[i].Distance(vPos);
      if (dDist < -dRadius)
         return resultOutside;
      else
      if (dDist < dRadius)
         result = resultIntersect;
   }

   return result;
}

/// \see http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-testing-boxes/
ViewFrustum3d::T_enIntersectResult ViewFrustum3d::IsBoxInside(const AABox& box) const throw()
{
   T_enIntersectResult result = resultInside;

   Vector3d vMin = box.Min(), vMax = box.Max();

   for (unsigned int i=0; i<6; i++)
   {
      // reset counters for corners in and out
      unsigned int in = 0, out = 0;

      // for each corner of the box do ...
      for (int k=0; k<8 && (in == 0 || out == 0); k++)
      {
         // construct corner point from vMin and vMax, depending of bits 1..3 of k
         // this way we get all corner points without constructing a list beforehand
         Vector3d vPt(
            (k & 1) ? vMax.X() : vMin.X(),
            (k & 2) ? vMax.Y() : vMin.Y(),
            (k & 4) ? vMax.Z() : vMin.Z());

         double dDist = m_aPlanes[i].Distance(vPt);
         if (dDist < 0)
            out++;
         else
            in++;
      }

      // all corners are out?
      if (in == 0)
         return resultOutside;
      else
      // some corners are out and others are in?
      if (out > 0)
         result = resultIntersect;
   }

   return result;
}

void ViewFrustum3d::RenderLines(const Vector3d& vPos, double dAngleDir, double dAngleUp,
                                double fov, double ratio, double nearDist, double farDist)
{
   FrustumCoordinates fc(vPos, dAngleDir, dAngleUp, fov, ratio, nearDist, farDist);

   OpenGL::PushedAttributes attrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);

   glDisable(GL_TEXTURE_2D);

   glColor3ub(255, 255, 255);

   glBegin(GL_LINE_LOOP);
   glVertex3dv(fc.ftl.Data());
   glVertex3dv(fc.ftr.Data());
   glVertex3dv(fc.fbr.Data());
   glVertex3dv(fc.fbl.Data());
   glEnd();

   glBegin(GL_LINE_LOOP);
   glVertex3dv(fc.ntl.Data());
   glVertex3dv(fc.ntr.Data());
   glVertex3dv(fc.nbr.Data());
   glVertex3dv(fc.nbl.Data());
   glEnd();

   glBegin(GL_LINES);
   glVertex3dv(fc.ntl.Data());
   glVertex3dv(fc.ftl.Data());

   glVertex3dv(fc.ntr.Data());
   glVertex3dv(fc.ftr.Data());

   glVertex3dv(fc.nbl.Data());
   glVertex3dv(fc.fbl.Data());

   glVertex3dv(fc.nbr.Data());
   glVertex3dv(fc.fbr.Data());
   glEnd();
}
