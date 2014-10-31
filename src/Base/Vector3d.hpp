//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Vector3d.hpp Vector class (3D)
//
#pragma once

// includes
#include "Math.hpp"

/// \brief 3d vector
/// \details x goes right, y goes up, z goes forward
template <typename T>
class Vector3T
{
public:
   /// ctor, taking vector components
   Vector3T(T dX = 0.0, T dY = 0.0, T dZ = 0.0) throw()
   {
      m_adVec[x] = dX;
      m_adVec[y] = dY;
      m_adVec[z] = dZ;
   }

   /// ctor, taking vector components as array
   Vector3T(const T adVec[3]) throw()
   {
      Set(adVec);
   }

   /// copy ctor
   Vector3T(const Vector3T& vec) throw()
   {
      Set(vec.m_adVec);
   }

   /// sets new vector components as array
   void Set(const T adVec[3]) throw()
   {
      memcpy(m_adVec, adVec, sizeof(m_adVec));
   }

   T X() const throw() { return m_adVec[x]; } ///< vector x value
   T Y() const throw() { return m_adVec[y]; } ///< vector y value
   T Z() const throw() { return m_adVec[z]; } ///< vector z value

   void X(T dX) throw() { m_adVec[x] = dX; } ///< sets vector x value
   void Y(T dY) throw() { m_adVec[y] = dY; } ///< sets vector y value
   void Z(T dZ) throw() { m_adVec[z] = dZ; } ///< sets vector z value


   // operators

   /// assign operator
   Vector3T& operator=(const Vector3T& vec) throw() { Set(vec.m_adVec); return *this; }

   /// add assignment
   Vector3T& operator+=(const Vector3T& vec) throw()
   {
      m_adVec[x] += vec.m_adVec[x];
      m_adVec[y] += vec.m_adVec[y];
      m_adVec[z] += vec.m_adVec[z];
      return *this;
   }

   /// subtract assignment
   Vector3T& operator-=(const Vector3T& vec) throw()
   {
      m_adVec[x] -= vec.m_adVec[x];
      m_adVec[y] -= vec.m_adVec[y];
      m_adVec[z] -= vec.m_adVec[z];
      return *this;
   }

   /// multiplication assignment with scale factor
   Vector3T& operator*=(T dScale) throw()
   {
      m_adVec[x] *= dScale;
      m_adVec[y] *= dScale;
      m_adVec[z] *= dScale;
      return *this;
   }


   // operations

   /// returns length of vector
   T Length() const throw() { return sqrt(m_adVec[x]*m_adVec[x] + m_adVec[y]*m_adVec[y] + m_adVec[z]*m_adVec[z]); }

   /// normalizes vector
   void Normalize() throw() { operator*=(1.0/Length()); }

   /// calculates inner (dot) product
   T Dot(const Vector3T& vec) const throw() { return m_adVec[x]*vec.m_adVec[x] + m_adVec[y]*vec.m_adVec[y] + m_adVec[z]*vec.m_adVec[z]; }

   /// calculates outer (cross) product
   void Cross(const Vector3T& lhs, const Vector3T& rhs) throw()
   {
      m_adVec[x] = lhs.m_adVec[y] * rhs.m_adVec[z] - lhs.m_adVec[z] * rhs.m_adVec[y];
      m_adVec[y] = lhs.m_adVec[z] * rhs.m_adVec[x] - lhs.m_adVec[x] * rhs.m_adVec[z];
      m_adVec[z] = lhs.m_adVec[x] * rhs.m_adVec[y] - lhs.m_adVec[y] * rhs.m_adVec[x];
   }

   /// calculates outer (cross) product
   Vector3T Cross(const Vector3T& rhs) const throw()
   {
      Vector3T vResult;
      vResult.Cross(*this, rhs);
      return vResult;
   }

   /// rotate vector around x axis
   void RotateX(T dAngle) throw()
   {
      T dTempY = m_adVec[y], dAngleRad = Deg2Rad(dAngle);
      m_adVec[y] = dTempY * cos(dAngleRad) - m_adVec[z]*sin(dAngleRad);
      m_adVec[z] = dTempY * sin(dAngleRad) + m_adVec[z]*cos(dAngleRad);
   }

   /// rotate vector around y axis
   void RotateY(T dAngle) throw()
   {
      T dTempX = m_adVec[x], dAngleRad = Deg2Rad(dAngle);
      m_adVec[x] =   dTempX * cos(dAngleRad) + m_adVec[z] * sin(dAngleRad);
      m_adVec[z] = - dTempX * sin(dAngleRad) + m_adVec[z] * cos(dAngleRad);
   }

   /// rotate vector around z axis
   void RotateZ(T dAngle) throw()
   {
      T dTempX = m_adVec[x], dAngleRad = Deg2Rad(dAngle);
      m_adVec[x] = dTempX * cos(dAngleRad) - m_adVec[y] * sin(dAngleRad);
      m_adVec[y] = dTempX * sin(dAngleRad) + m_adVec[y] * cos(dAngleRad);
   }


   // data access

   /// access to vector data; const version
   const T* Data() const throw() { return m_adVec; }

   /// access to vector data; non-const version
   T* Data() throw() { return m_adVec; }

private:
   /// enum to access vector components by name
   enum {
      x = 0,
      y = 1,
      z = 2
   };

   /// vector components
   T m_adVec[3];
};

// free functions

/// unary minus
template <typename T>
inline Vector3T<T> operator-(const Vector3T<T>& vec) throw()
{
   return Vector3T<T>(-vec.X(), -vec.Y(), -vec.Z());
};

/// add operator
template <typename T>
inline Vector3T<T> operator+(const Vector3T<T>& v1, const Vector3T<T>& v2) throw()
{
   return Vector3T<T>(v1.X() + v2.X(), v1.Y() + v2.Y(), v1.Z() + v2.Z());
}

/// subtract operator
template <typename T>
inline Vector3T<T> operator-(const Vector3T<T>& v1, const Vector3T<T>& v2) throw()
{
   return Vector3T<T>(v1.X() - v2.X(), v1.Y() - v2.Y(), v1.Z() - v2.Z());
}

/// scalar multiplication, vector * scalar
template <typename T>
inline Vector3T<T> operator*(const Vector3T<T>& vec, const T dScale) throw()
{
   Vector3T<T> vRet(vec);
   vRet *= dScale;
   return vRet;
}

/// scalar multiplication, scalar * vector
template <typename T>
inline Vector3T<T> operator*(const T dScale, const Vector3T<T>& vec) throw()
{
   Vector3T<T> vRet(vec);
   vRet *= dScale;
   return vRet;
}

/// division by scalar
template <typename T>
inline Vector3T<T> operator/(const Vector3T<T>& vec, const T d)
{
   return Vector3T<T>(vec.X()/d, vec.Y()/d, vec.Z()/d);
}

typedef Vector3T<double> Vector3d;
typedef Vector3T<float> Vector3f;
