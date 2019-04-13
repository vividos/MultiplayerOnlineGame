//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Quaternion4.hpp Quaternion class
//
#pragma once

// includes
#include "Vector3.hpp"

/// quaternion
class BASE_DECLSPEC Quaternion4d
{
public:
   /// ctor, taking vector and w component
   Quaternion4d(const Vector3d& vXyz, double dW)
      :m_vXyz(vXyz),
       m_dW(dW)
   {
   }

   /// ctor, taking components
   Quaternion4d(double dX = 0.0, double dY = 0.0, double dZ = 0.0, double dW = 0.0)
      :m_vXyz(dX, dY, dZ),
       m_dW(dW)
   {
   }

   /// calculates length of quaternion
   double Length() const;

   /// returns xyz components
   const Vector3d& Xyz() const { return m_vXyz; }

   /// returns w component
   double W() const { return m_dW; }

   /// scales quaternion to unit length
   void Normalize();

   /// converts quaternion to axis and rotation angle
   void ToAxisAngle(Vector3d& vAxis, double& dAngle) const;

   /// calculate quaternion from angles
   void FromAngles(const double adAngles[3]);

   /// spherical linear interpolation between two quaternions
   static Quaternion4d Slerp(const Quaternion4d& p, const Quaternion4d& q, double t);

private:
   /// read-write access to components
   double& operator[](size_t i)
   {
      ATLASSERT(i<4);
      return i<3 ? m_vXyz.Data()[i] : m_dW;
   }
   /// read-only access to components
   const double& operator[](size_t i) const
   {
      ATLASSERT(i<4);
      return i<3 ? m_vXyz.Data()[i] : m_dW;
   }

private:
   /// xyz components
   Vector3d m_vXyz;

   /// w component
   double m_dW;
};
