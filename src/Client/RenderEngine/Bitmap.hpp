//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Bitmap.hpp Bitmap class
//
#pragma once

/// includes
#include "RenderEngineCommon.hpp"
#include "Color.hpp"
#include <vector>

/// bitmap class
class RENDERENGINE_DECLSPEC Bitmap
{
public:
   /// ctor; takes resolution and RGBA data
   Bitmap(unsigned int xres, unsigned int yres, const unsigned char* data = NULL)
      :m_vecData(xres*yres),
       m_xres(xres),
       m_yres(yres)
   {
      if (data != NULL)
      {
         static_assert(sizeof(Color) == 4, "Color must have a size of 4");
         memcpy(&m_vecData[0].m_color[0], data, sizeof(Color) * xres * yres);
      }
   }

   /// ctor; takes resolution and fill color
   Bitmap(unsigned int xres, unsigned int yres, Color c)
      :m_vecData(xres*yres, c),
       m_xres(xres),
       m_yres(yres)
   {
   }

   /// ctor; takes resolution and RGBA color data
   Bitmap(unsigned int xres, unsigned int yres, const Color* c)
      :m_vecData(c, c + xres*yres),
       m_xres(xres),
       m_yres(yres)
   {
      static_assert(sizeof(Color) == 4, "Color must have a size of 4");
   }

   /// dtor
   ~Bitmap() {}

   /// returns x resolution
   unsigned int XRes() const { return m_xres; }
   /// returns < resolution
   unsigned int YRes() const { return m_yres; }

   /// sets single pixel
   void Set(unsigned int x, unsigned int y, Color c);

   /// fills rectangle with color
   void Fill(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, Color c);

   /// copies pixels from source bitmap
   void Copy(const Bitmap& from, int tox, int toy,
      unsigned int left = 0, unsigned int top = 0, unsigned int bottom = 0, unsigned int right = 0);

   /// access to underlying data; non-const version
   Color* Data() { return &m_vecData[0]; }

   /// access to underlying data; const version
   const Color* Data() const { return &m_vecData[0]; }

private:
   unsigned int m_xres; ///< x resolution
   unsigned int m_yres; ///< y resolution
   std::vector<Color> m_vecData; ///< color data
};
