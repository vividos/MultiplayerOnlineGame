//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Bitmap.cpp Bitmap class
//

/// includes
#include "StdAfx.h"
#include "Bitmap.hpp"

void Bitmap::Set(unsigned int x, unsigned int y, Color c)
{
   m_vecData[y * m_xres + x] = c;
}

void Bitmap::Fill(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom, Color c)
{
   ATLASSERT(bottom >= top);
   ATLASSERT(bottom <= m_yres);
   ATLASSERT(right >= left);
   ATLASSERT(right <= m_xres);

   for (unsigned int y = top; y<bottom; y++)
   {
      std::vector<Color>::iterator iter = m_vecData.begin() + y*m_xres + left;
      for(unsigned int x = 0, xmax = right-left; x<xmax; x++)
         *iter++ = c;
   }
}

void Bitmap::Copy(const Bitmap& from, int tox, int toy,
   unsigned int left, unsigned int top, unsigned int bottom, unsigned int right)
{
   // when using with default param, set right and bottom values
   if (top == 0 && bottom == 0)
      bottom = from.m_yres;

   if (left == 0 && right == 0)
      right = from.m_xres;

   // fix right and bottom values that are too large
   if (right > from.m_xres)
      right = from.m_xres;

   if (bottom > from.m_yres)
      bottom = from.m_yres;

   // fix target start point when negative
   unsigned int ystart = 0;
   if (toy < 0)
   {
      toy = 0;
      ystart = static_cast<unsigned int>(-toy);
      top += ystart;
      if (top > bottom)
         top = 0;
   }

   if (tox < 0)
   {
      tox = 0;
      unsigned int xstart = static_cast<unsigned int>(-tox);
      left += xstart;
      if (left > right)
         left = 0;
   }

   // calculate height and width
   unsigned int width = right - left;
   unsigned int height = bottom - top;

   if (width == 0 || height == 0)
      return; // nothing to do; singular rectangle

   // fix width and height when not fitting to our resolution
   if (tox >= static_cast<int>(m_xres) || toy >= static_cast<int>(m_yres))
      return; // nothing to do; insert point is outside destination rect

   if (tox + width > m_xres)
      width = m_xres - tox;

   if (toy + height > m_yres)
      height = m_yres - toy;

   // copy that stuff
   for (unsigned int y = ystart; y<height; y++)
   {
      std::vector<Color>::const_iterator iter_from = from.m_vecData.begin() + (y+top) * from.m_xres + left;
      std::vector<Color>::iterator iter_to = m_vecData.begin() + (y+toy)*m_xres + tox;
      std::copy(iter_from, iter_from + width, iter_to);
   }
}
