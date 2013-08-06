//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Vector2d.hpp Vector class (2D)
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
   Vector2d() throw()
      :m_x(0.0),
       m_y(0.0)
   {
   }

   /// copy ctor
   Vector2d(const Vector2d& vec) throw()
      :m_x(vec.m_x),
       m_y(vec.m_y)
   {
   }

   /// ctor with x and y coordinates
   Vector2d(double x, double y) throw()
      :m_x(x),
       m_y(y)
   {
   }

   // operators

   /// assign operator
   Vector2d& operator=(const Vector2d& vec) throw()
   {
      if (&vec != this)
      {
         m_x = vec.m_x;
         m_y = vec.m_y;
      }
      return *this;
   }

   /// add assignment
   Vector2d& operator+=(const Vector2d& vec) throw()
   {
      m_x += vec.m_x;
      m_y += vec.m_y;
      return *this;
   }

   /// subtract assignment
   Vector2d& operator-=(const Vector2d& vec) throw()
   {
      m_x -= vec.m_x;
      m_y -= vec.m_y;
      return *this;
   }

   /// multiplication assignment with scale factor
   Vector2d& operator*=(double dScale) throw()
   {
      m_x *= dScale;
      m_y *= dScale;
      return *this;
   }

   // methods

   double X() const throw() { return m_x; } ///< returns x value
   double Y() const throw() { return m_y; } ///< returns y value

   void X(double x) throw() { m_x = x; } ///< sets vector x value
   void Y(double y) throw() { m_y = y; } ///< sets vector y value

   /// sets vector by giving polar coordinates (angle in degree, not radians)
   void SetPolar(double dLen, double dAngle) throw()
   {
      m_x = dLen * cos(Deg2Rad(dAngle));
      m_y = dLen * sin(Deg2Rad(dAngle));
   }

   /// returns length of vector
   double Length() const throw()
   {
      return sqrt(m_x * m_x + m_y * m_y);
   }

   /// returns polar angle of vector, in degrees
   double PolarAngle() const throw()
   {
      return AngleInRange(Rad2Deg(std::atan2(m_y, m_x)));
   }

   /// normalizes vector
   void Normalize() throw()
   {
      this->operator*=(1.0/Length());
   }

   /// calculates the inner (dot) product
   double Dot(const Vector2d& vec) const throw()
   {
      return m_x * vec.m_x + m_y * vec.m_y;
   }

   /// rotates vector; angle in degree
   void Rotate(double dAngle) throw()
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
inline Vector2d operator-(const Vector2d& vec) throw()
{
   return Vector2d(-vec.X(), -vec.Y());
};

/// add operator
inline Vector2d operator+(const Vector2d& v1, const Vector2d& v2) throw()
{
   return Vector2d(v1.X()+v2.X(), v1.Y()+v2.Y());
}

/// subtract operator
inline Vector2d operator-(const Vector2d& v1, const Vector2d& v2) throw()
{
   return Vector2d(v1.X()-v2.X(), v1.Y()-v2.Y());
}

/// scalar multiplication, vector * scalar
inline Vector2d operator*(const Vector2d& vec, const double dScale) throw()
{
   Vector2d vRet(vec);
   vRet *= dScale;
   return vRet;
}

/// scalar multiplication, scalar * vector
inline Vector2d operator*(const double dScale, const Vector2d& vec) throw()
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
