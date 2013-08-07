//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Math.hpp Math defines and functions
//
#pragma once

// includes
#include <cmath>


/// pi constant
const double g_dPi = 3.141592653589793;

/// converts angle from degree values (0°..360°) to radians (0..2pi)
inline double Deg2Rad(double a) throw() { return (static_cast<double>(a)/180.0)*g_dPi; }

/// converts angle from randians (0..2pi) values to degrees (0°..360°)
inline double Rad2Deg(double a) throw() { return (static_cast<double>(a)/g_dPi)*180.0; }

/// keeps angle in range of [0°;360°[
inline double AngleInRange(double dDegrees) throw()
{
   return dDegrees < 0 ? (360.0-fmod(-dDegrees, 360.0)) : fmod(dDegrees, 360.0);
}

/// compares two double values and return if they're equal, with an epsilon margin
inline bool DoublesEqual(double d0, double d1, double epsilon = 1e-6) throw()
{
   return fabs(d0 - d1) < epsilon;
}

/// interpolates between two values of type T
/// \tparam T data type
template <typename T>
T Interpolate(T val1, T val2, double dDelta)
{
   ATLASSERT(dDelta >= 0.0 && dDelta <= 1.0);
   return T(val1 * (1.0-dDelta)) + T(val2 * dDelta);
}


/// \brief linear transformation that converts from one range to another
/// \tparam T data type
template <typename T>
struct LinearTransform
{
   /// ctor
   LinearTransform(T minFrom, T maxFrom, T minTo, T maxTo)
      :m_add(minTo),
       m_sub(minFrom),
       m_mul((maxTo-minTo)/(maxFrom-minFrom))
   {
   }

   /// returns linear transformed value from range
   T operator()(T val) const throw()
   {
      return T((val - m_sub) * m_mul + m_add);
   }

   T m_add; ///< offset to add
   T m_sub; ///< offset
   T m_mul; ///< multiplicator
};


/// \brief calculates greatest common divisor
/// \tparam T data type
template <typename T>
T GreatestCommonDivisor(T n, T m) throw()
{
   T rest = m;
   while (rest != 0)
   {
      rest = fmod(n, m);
      n = m;
      m = rest;
   }
   return n;
}

/// calculates least common multiple
/// \tparam T data type
template <typename T>
T LeastCommonMultiple(T n, T m) throw()
{
   return (n * m) / GreatestCommonDivisor<T>(n, m);
}


/// determines if value is power of two
/// \tparam T data type (must be integral type)
/// \details from http://graphics.stanford.edu/~seander/bithacks.html
template <typename T>
bool IsPowerOfTwo(T v)
{
   //static_assert(true == boost::type_traits::is_integral_type(T), "T must be integral type");
   return v && !(v & (v - 1));
}

/// calculates next power of two for given n
inline unsigned int NextPowerOfTwo(unsigned int n)
{
   unsigned int i;
   for (i=1; i<32 && (1u<<i) < n; i++);
   return 1u << i;
}

/// clamps value to range
template <typename T>
T Clamp(T min, T max, T value)
{
   //return std::min(std::max(value, min), max);
   if (value < min)
      value = min;
   if (value > max)
      value = max;
   return value;
}

/// determines if a value is in range
template <typename T>
bool InRange(T min, T max, T value)
{
   if (value < min)
      return false;
   if (value > max)
      return false;
   return true;
}
