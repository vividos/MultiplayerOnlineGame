//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Color.hpp Color class
//
#pragma once

// includes
#include "Math.hpp"

/// RGBA color
struct Color
{
   /// ctor
   Color() throw() {}

   /// ctor that takes array of RGBA components
   Color(const unsigned char c[4]) throw() { memcpy(m_color, c, sizeof(m_color)); }

   /// ctor that takes RGBA components as separate parameters
   Color(unsigned char cRed, unsigned char cGreen, unsigned char cBlue, unsigned char cAlpha = 255) throw()
   {
      m_color[red] = cRed;
      m_color[green] = cGreen;
      m_color[blue] = cBlue;
      m_color[alpha] = cAlpha;
   }

   static Color White() throw() { unsigned char c[4] = { 255, 255, 255, 255}; return Color(c); } ///< predefined color white
   static Color Black() throw() { unsigned char c[4] = {   0,   0,   0, 255}; return Color(c); } ///< predefined color black
   static Color Red() throw() {   unsigned char c[4] = { 255,   0,   0, 255}; return Color(c); } ///< predefined color red
   static Color Green() throw() { unsigned char c[4] = {   0, 255,   0, 255}; return Color(c); } ///< predefined color green
   static Color Blue() throw() {  unsigned char c[4] = {   0,   0, 255, 255}; return Color(c); } ///< predefined color blue
   static Color Transparent() throw() {   unsigned char c[4] = { 0, 0, 0, 0}; return Color(c); } ///< predefined color "transparent"

   /// color channel enum to access m_color
   enum T_enColorChannel
   {
      red = 0,    ///< red value
      green = 1,  ///< green value
      blue = 2,   ///< blue value
      alpha = 3   ///< alpha value
   };

   /// RGBA color components
   unsigned char m_color[4];

   /// calculates luminance with fixed factors
   unsigned char Luminance() const throw()
   {
      return static_cast<unsigned char>(
         0.299 * m_color[red] + 0.587 * m_color[green] + 0.114 *  m_color[blue] );
   }

   /// \brief calculates RGB color values to HSL (hue, saturation, luminance)
   /// \see http://en.wikipedia.org/wiki/HSL_and_HSV
   /// \see http://130.113.54.154/~monger/hsl-rgb.html
   void ToHSL(float hsl[3])
   {
      unsigned char ucM = std::max(std::max(m_color[red], m_color[green]), m_color[blue]);
      unsigned char ucm = std::min(std::min(m_color[red], m_color[green]), m_color[blue]);
      ATLASSERT(ucM >= ucm);

      float M = ucM / 256.f;
      float m = ucm / 256.f;

      // calc L
      hsl[2] = (M + m) / 2.f;

      // grey?
      if (M == m)
      {
         hsl[0] = hsl[1] = 0.f; // h, s
         return;
      }

      // calc S
      if (hsl[2] < 0.5)
         hsl[1] = (M - m)/(M + m);
      else
         hsl[1] = (M - m)/(2.f - M - m);

      // calc H
      if (m_color[red] == ucM)
         hsl[0] = ((m_color[green] - m_color[blue]) / 256.f) / (M-m);
      if (m_color[green] == ucM)
         hsl[0] = 2.f + ((m_color[blue] - m_color[red]) / 256.f) / (M-m);
      if (m_color[blue] == ucM)
         hsl[0] = 4.f + ((m_color[red] - m_color[green]) / 256.f) / (M-m);

      // normalize H
      hsl[0] /= 6.0;
      if (hsl[0] < 0.0)
         hsl[0] += 1.0;
   }
};

/// \brief interpolates between two colors
/// specialisation of Interpolate function in Math.hpp
template <>
inline Color Interpolate(Color val1, Color val2, double dDelta)
{
   return Color(
      Interpolate(val1.m_color[Color::red],    val2.m_color[Color::red],    dDelta),
      Interpolate(val1.m_color[Color::green],  val2.m_color[Color::green],  dDelta),
      Interpolate(val1.m_color[Color::blue],   val2.m_color[Color::blue],   dDelta),
      Interpolate(val1.m_color[Color::alpha],  val2.m_color[Color::alpha],  dDelta));
}
