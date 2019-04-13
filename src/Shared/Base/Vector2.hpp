//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Vector2.hpp Vector class (2D)
//
#pragma once

// includes
#include "Math.hpp"

/// \brief 2d vector
/// \details The class represents a 2 dimensional vector with x and y coordinates.
class Vector2d
{
public:
   /// ctor
   Vector2d()
      :m_x(0.0),
       m_y(0.0)
   {
   }

   /// copy ctor
   Vector2d(const Vector2d& vec)
      :m_x(vec.m_x),
       m_y(vec.m_y)
   {
   }

   /// ctor with x and y coordinates
   Vector2d(double x, double y)
      :m_x(x),
       m_y(y)
   {
   }

   // operators

   /// assign operator
   Vector2d& operator=(const Vector2d& vec)
   {
      if (&vec != this)
      {
         m_x = vec.m_x;
         m_y = vec.m_y;
      }
      return *this;
   }

   /// add assignment
   Vector2d& operator+=(const Vector2d& vec)
   {
      m_x += vec.m_x;
      m_y += vec.m_y;
      return *this;
   }

   /// subtract assignment
   Vector2d& operator-=(const Vector2d& vec)
   {
      m_x -= vec.m_x;
      m_y -= vec.m_y;
      return *this;
   }

   /// multiplication assignment with scale factor
   Vector2d& operator*=(double dScale)
   {
      m_x *= dScale;
      m_y *= dScale;
      return *this;
   }

   // methods

   double X() const { return m_x; } ///< returns x value
   double Y() const { return m_y; } ///< returns y value

   void X(double x) { m_x = x; } ///< sets vector x value
   void Y(double y) { m_y = y; } ///< sets vector y value

   /// sets vector by giving polar coordinates (angle in degree, not radians)
   void SetPolar(double dLen, double dAngle)
   {
      m_x = dLen * cos(Deg2Rad(dAngle));
      m_y = dLen * sin(Deg2Rad(dAngle));
   }

   /// returns length of vector
   double Length() const
   {
      return sqrt(m_x * m_x + m_y * m_y);
   }

   /// returns polar angle of vector, in degrees
   double PolarAngle() const
   {
      return AngleInRange(Rad2Deg(std::atan2(m_y, m_x)));
   }

   /// normalizes vector
   void Normalize()
   {
      this->operator*=(1.0/Length());
   }

   /// calculates the inner (dot) product
   double Dot(const Vector2d& vec) const
   {
      return m_x * vec.m_x + m_y * vec.m_y;
   }

   /// rotates vector; angle in degree
   void Rotate(double dAngle)
   {
      double temp = m_x * cos(Deg2Rad(dAngle)) - m_y * sin(Deg2Rad(dAngle));
      m_y =         m_x * sin(Deg2Rad(dAngle)) + m_y * cos(Deg2Rad(dAngle));
      m_x = temp;
   }

private:
   double m_x; ///< x element
   double m_y; ///< y element
};


// free functions

/// unary minus
inline Vector2d operator-(const Vector2d& vec)
{
   return Vector2d(-vec.X(), -vec.Y());
};

/// add operator
inline Vector2d operator+(const Vector2d& v1, const Vector2d& v2)
{
   return Vector2d(v1.X()+v2.X(), v1.Y()+v2.Y());
}

/// subtract operator
inline Vector2d operator-(const Vector2d& v1, const Vector2d& v2)
{
   return Vector2d(v1.X()-v2.X(), v1.Y()-v2.Y());
}

/// scalar multiplication, vector * scalar
inline Vector2d operator*(const Vector2d& vec, const double dScale)
{
   Vector2d vRet(vec);
   vRet *= dScale;
   return vRet;
}

/// scalar multiplication, scalar * vector
inline Vector2d operator*(const double dScale, const Vector2d& vec)
{
   Vector2d vRet(vec);
   vRet *= dScale;
   return vRet;
}

/// division by scalar
inline Vector2d operator/(const Vector2d& vec, const double d)
{
   return Vector2d(vec.X()/d, vec.Y()/d);
}

/// calculates if a square intersects a circle
/// \see http://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
inline bool IntersectSquareCircle(const Vector2d& vSquareBase, double dSizeSquare,
                           const Vector2d& vCircleCenter, double dRadius)
{
   double closestX = Clamp(vSquareBase.X(), vSquareBase.X()+dSizeSquare, vCircleCenter.X());
   double closestY = Clamp(vSquareBase.Y(), vSquareBase.Y()+dSizeSquare, vCircleCenter.Y());

   // Calculate the distance between the circle's center and this closest point
   double distanceX = vCircleCenter.X() - closestX;
   double distanceY = vCircleCenter.Y() - closestY;

   // If the distance is less than the circle's radius, an intersection occurs
   double distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
   return distanceSquared < (dRadius * dRadius);
}
