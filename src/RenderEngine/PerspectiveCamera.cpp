//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file PerspectiveCamera.cpp Perspective camera class
//

// includes
#include "StdAfx.h"
#include "PerspectiveCamera.hpp"
#include <SDL.h>
#include <cmath>

/// default Field Of View angle
const double g_dAngleFieldOfView = 75.0;

PerspectiveCamera::PerspectiveCamera()
:m_dAngleDirection(0.0),
 m_dAngleUp(0.0),
 m_dNearPlaneDist(1.0),
 m_dFarPlaneDist(100.0)
{
}

void PerspectiveCamera::SetPosition(const Vector3d& vPos, const Vector3d& vLookAt)
{
   Vector3d vDir = vLookAt - vPos;
   if (vDir.Length() < 1e-6)
      vDir = Vector3d(0.0, 0.0, 1.0);

   double dAngleDirection = Rad2Deg(atan2(vDir.X(), -vDir.Z()));
   double dAngleUp = Rad2Deg(atan2(vDir.Y(), -vDir.Z()));

   SetPosition(vPos, dAngleDirection, dAngleUp);
}

void PerspectiveCamera::SetPosition(const Vector3d& vPos, double dAngleDirection, double dAngleUp)
{
   m_vPos = vPos;
   m_dAngleDirection = dAngleDirection;
   m_dAngleUp = dAngleUp;

   // set up projection matrix
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   GLint viewport[4];
   glGetIntegerv(GL_VIEWPORT, viewport);

   double dRatio = GLdouble(viewport[2]) / viewport[3];

   gluPerspective(g_dAngleFieldOfView, dRatio, m_dNearPlaneDist, m_dFarPlaneDist);

   glRotated(-dAngleUp, 1.0, 0.0, 0.0); // rotate around x axis
   glRotated(dAngleDirection, 0.0, 1.0, 0.0); // rotate around y axis

   glMatrixMode(GL_MODELVIEW);
}

void PerspectiveCamera::Use()
{
   glTranslated(-m_vPos.X(), -m_vPos.Y(), -m_vPos.Z());
}

void PerspectiveCamera::SetNearFarPlaneDistance(double dNearPlaneDist, double dFarPlaneDist)
{
   m_dNearPlaneDist = dNearPlaneDist;
   m_dFarPlaneDist = dFarPlaneDist;
}

ViewFrustum3d PerspectiveCamera::GetViewFrustum() const throw()
{
   GLint viewport[4];
   glGetIntegerv(GL_VIEWPORT, viewport);

   double dRatio = GLdouble(viewport[2]) / viewport[3];

   return ViewFrustum3d(m_vPos,
      180.0-m_dAngleDirection,
      m_dAngleUp,
      g_dAngleFieldOfView,
      dRatio,
      m_dNearPlaneDist,
      m_dFarPlaneDist);
}
