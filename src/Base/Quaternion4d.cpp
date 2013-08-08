//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Quaternion4d.cpp Quaternion class
//

// includes
#include "StdAfx.h"
#include "Quaternion4d.hpp"
#include "Math.hpp"

double Quaternion4d::Length() const throw()
{
   double dLen = m_vXyz.Length();
   return std::sqrt(m_dW * m_dW + dLen * dLen);
}

void Quaternion4d::Normalize() throw()
{
   double dScale = 1.0 / Length();
   m_vXyz *= dScale;
   m_dW *= dScale;
}

void Quaternion4d::ToAxisAngle(Vector3d& vAxis, double& dAngle) const throw()
{
   Quaternion4d q(*this);
   if (q.W() > 1.0)
      q.Normalize();

   dAngle = 2.0 * std::acos(q.W());
   double denom = std::sqrt(1.0 - q.W() * q.W());

   if (denom > 1e-4)
      vAxis = q.Xyz() / denom;
   else
      vAxis = Vector3d(1.0, 0.0, 0.0); // unit x vector
}

void Quaternion4d::FromAngles(const double adAngles[3])
{
   double angle = adAngles[2] * 0.5;
   double sy = sin(angle);
   double cy = cos(angle);

   angle = adAngles[1] * 0.5;
   double sp = sin(angle);
   double cp = cos(angle);

   angle = adAngles[0] * 0.5;
   double sr = sin(angle);
   double cr = cos(angle);

   m_vXyz.X(sr*cp*cy-cr*sp*sy);
   m_vXyz.Y(cr*sp*cy+sr*cp*sy);
   m_vXyz.Z(cr*cp*sy-sr*sp*cy);
   m_dW =   cr*cp*cy+sr*sp*sy;
}

Quaternion4d Quaternion4d::Slerp(const Quaternion4d& p, const Quaternion4d& q_, double t)
{
   Quaternion4d q(q_); // need non-const version when inverting

   // decide if one of the quaternions is backwards
   double a = 0;
   double b = 0;
   for (size_t i = 0; i < 4; i++)
   {
      a += (p[i]-q[i])*(p[i]-q[i]);
      b += (p[i]+q[i])*(p[i]+q[i]);
   }
   if (a > b)
   {
      for (size_t i = 0; i < 4; i++)
      {
         q[i] = -q[i];
      }
   }

   double cosom = p[0]*q[0] + p[1]*q[1] + p[2]*q[2] + p[3]*q[3];

   Quaternion4d qt;
   double sclp, sclq;

   if ((1.0 + cosom) > 0.00000001)
   {
      if ((1.0 - cosom) > 0.00000001)
      {
         double omega = acos( cosom );
         double sinom = sin( omega );
         sclp = sin( (1.0 - t)*omega) / sinom;
         sclq = sin( t*omega ) / sinom;
      }
      else
      {
         sclp = 1.0 - t;
         sclq = t;
      }
      for (size_t i = 0; i < 4; i++)
      {
         qt[i] = sclp * p[i] + sclq * q[i];
      }
   }
   else
   {
      qt[0] = -p[1];
      qt[1] = p[0];
      qt[2] = -p[3];
      qt[3] = p[2];
      sclp = sin( (1.0 - t) * 0.5 * g_dPi);
      sclq = sin( t * 0.5 * g_dPi);
      for (size_t i = 0; i < 3; i++) {
         qt[i] = sclp * p[i] + sclq * qt[i];
      }
   }
   return qt;
}
