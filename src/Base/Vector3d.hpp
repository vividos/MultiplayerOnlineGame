//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Vector3d.hpp Vector class (3D)
//
#pragma once

// includes
#include "Math.hpp"

/// \brief 3d vector
/// \details x goes right, y goes up, z goes forward
class Vector3d
{
public:
   /// ctor, taking vector components
   Vector3d(double dX = 0.0, double dY = 0.0, double dZ = 0.0) throw()
   {
      m_adVec[x] = dX;
      m_adVec[y] = dY;
      m_adVec[z] = dZ;
   }

   /// ctor, taking vector components as array
   Vector3d(const double adVec[3]) throw()
   {
      Set(adVec);
   }

   /// copy ctor
   Vector3d(const Vector3d& vec) throw()
   {
      Set(vec.m_adVec);
   }

   /// sets new vector components as array
   void Set(const double adVec[3]) throw()
   {
      memcpy(m_adVec, adVec, sizeof(m_adVec));
   }

   double X() const throw() { return m_adVec[x]; } ///< vector x value
   double Y() const throw() { return m_adVec[y]; } ///< vector y value
   double Z() const throw() { return m_adVec[z]; } ///< vector z value

   void X(double dX) throw() { m_adVec[x] = dX; } ///< sets vector x value
   void Y(double dY) throw() { m_adVec[y] = dY; } ///< sets vector y value
   void Z(double dZ) throw() { m_adVec[z] = dZ; } ///< sets vector z value


   // operators

   /// assign operator
   Vector3d& operator=(const Vector3d& vec) throw() { Set(vec.m_adVec); return *this; }

   /// add assignment
   Vector3d& operator+=(const Vector3d& vec) throw()
   {
      m_adVec[x] += vec.m_adVec[x];
      m_adVec[y] += vec.m_adVec[y];
      m_adVec[z] += vec.m_adVec[z];
      return *this;
   }

   /// subtract assignment
   Vector3d& operator-=(const Vector3d& vec) throw()
   {
      m_adVec[x] -= vec.m_adVec[x];
      m_adVec[y] -= vec.m_adVec[y];
      m_adVec[z] -= vec.m_adVec[z];
      return *this;
   }

   /// multiplication assignment with scale factor
   Vector3d& operator*=(double dScale) throw()
   {
      m_adVec[x] *= dScale;
      m_adVec[y] *= dScale;
      m_adVec[z] *= dScale;
      return *this;
   }


   // operations

   /// returns length of vector
   double Length() const throw() { return sqrt(m_adVec[x]*m_adVec[x] + m_adVec[y]*m_adVec[y] + m_adVec[z]*m_adVec[z]); }

   /// normalizes vector
   void Normalize() throw() { operator*=(1.0/Length()); }

   /// calculates inner (dot) product
   double Dot(const Vector3d& vec) const throw() { return m_adVec[x]*vec.m_adVec[x] + m_adVec[y]*vec.m_adVec[y] + m_adVec[z]*vec.m_adVec[z]; }

   /// calculates outer (cross) product
   void Cross(const Vector3d& lhs, const Vector3d& rhs) throw()
   {
      m_adVec[x] = lhs.m_adVec[y] * rhs.m_adVec[z] - lhs.m_adVec[z] * rhs.m_adVec[y];
      m_adVec[y] = lhs.m_adVec[z] * rhs.m_adVec[x] - lhs.m_adVec[x] * rhs.m_adVec[z];
      m_adVec[z] = lhs.m_adVec[x] * rhs.m_adVec[y] - lhs.m_adVec[y] * rhs.m_adVec[x];
   }

   /// calculates outer (cross) product
   Vector3d Cross(const Vector3d& rhs) const throw()
   {
      Vector3d vResult;
      vResult.Cross(*this, rhs);
      return vResult;
   }

   /// rotate vector around x axis
   void RotateX(double dAngle) throw()
   {
      double dTempY = m_adVec[y], dAngleRad = Deg2Rad(dAngle);
      m_adVec[y] = dTempY * cos(dAngleRad) - m_adVec[z]*sin(dAngleRad);
      m_adVec[z] = dTempY * sin(dAngleRad) + m_adVec[z]*cos(dAngleRad);
   }

   /// rotate vector around y axis
   void RotateY(double dAngle) throw()
   {
      double dTempX = m_adVec[x], dAngleRad = Deg2Rad(dAngle);
      m_adVec[x] =   dTempX * cos(dAngleRad) + m_adVec[z] * sin(dAngleRad);
      m_adVec[z] = - dTempX * sin(dAngleRad) + m_adVec[z] * cos(dAngleRad);
   }

   /// rotate vector around z axis
   void RotateZ(double dAngle) throw()
   {
      double dTempX = m_adVec[x], dAngleRad = Deg2Rad(dAngle);
      m_adVec[x] = dTempX * cos(dAngleRad) - m_adVec[y] * sin(dAngleRad);
      m_adVec[y] = dTempX * sin(dAngleRad) + m_adVec[y] * cos(dAngleRad);
   }


   // data access

   /// access to vector data; const version
   const double* Data() const throw() { return m_adVec; }

   /// access to vector data; non-const version
   double* Data() throw() { return m_adVec; }

private:
   /// enum to access vector components by name
   enum {
      x = 0,
      y = 1,
      z = 2
   };

   /// vector components
   double m_adVec[3];
};

// free functions

/// unary minus
inline Vector3d operator-(const Vector3d& vec) throw()
{
   return Vector3d(-vec.X(), -vec.Y(), -vec.Z());
};

/// add operator
inline Vector3d operator+(const Vector3d& v1, const Vector3d& v2) throw()
{
   return Vector3d(v1.X()+v2.X(), v1.Y()+v2.Y(), v1.Z()+v2.Z());
}

/// subtract operator
inline Vector3d operator-(const Vector3d& v1, const Vector3d& v2) throw()
{
   return Vector3d(v1.X()-v2.X(), v1.Y()-v2.Y(), v1.Z()-v2.Z());
}

/// scalar multiplication, vector * scalar
inline Vector3d operator*(const Vector3d& vec, const double dScale) throw()
{
   Vector3d vRet(vec);
   vRet *= dScale;
   return vRet;
}

/// scalar multiplication, scalar * vector
inline Vector3d operator*(const double dScale, const Vector3d& vec) throw()
{
   Vector3d vRet(vec);
   vRet *= dScale;
   return vRet;
}

/// division by scalar
inline Vector3d operator/(const Vector3d& vec, const double d)
{
   return Vector3d(vec.X()/d, vec.Y()/d, vec.Z()/d);
}
