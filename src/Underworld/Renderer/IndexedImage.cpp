//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Renderer/IndexedImage.cpp Palette indexed image
//

// includes
#include "stdafx.h"
#include "IndexedImage.hpp"

// IndexedImage methods

void IndexedImage::Create(unsigned int width, unsigned int height)
{
   xres = width; yres = height;
   m_vecPixels.resize(width*height);
}

/// when image to paste and image that is pasted is the same one, then
/// non-transparent pastes are only successful when the source and target
/// areas don't overlap
void IndexedImage::PasteRect(const IndexedImage& fromImage, unsigned int fromx, unsigned int fromy,
   unsigned width, unsigned height, unsigned int destx, unsigned int desty, bool transparent)
{
   // get resolution and pixel vectors
   unsigned int sxres = fromImage.XRes();
   unsigned int syres = fromImage.YRes();

   // check if width and height is in range
   if (width > sxres) width = sxres;
   if (height > syres) height = syres;

   // adjust source image if pasting would cross dest image borders
   if (destx + width > xres) width = xres - destx;
   if (desty + height > yres) height = yres - desty;

   // get source and dest pointer
   const Uint8* src = &fromImage.Pixels()[fromx + fromy * sxres];
   Uint8* dest = &Pixels()[destx + desty * xres];

   if (!transparent)
   {
      // non-transparent paste
      for (unsigned int y=0; y<height; y++)
         memcpy(&dest[y*xres], &src[y*sxres], width);
   }
   else
   {
      // paste that omits transparent parts
      for (unsigned int y=0; y<height; y++)
      {
         const Uint8* src_line = &src[y*sxres];
         Uint8* dest_line = &dest[y*xres];

         for(unsigned int x=0; x<width; x++)
         {
            Uint8 pixel = src_line[x];
            if (pixel!=0)
               dest_line[x] = pixel;
         }
      }
   }
}

void IndexedImage::FillRect(unsigned int startx, unsigned int starty,
   unsigned int width, unsigned int height, unsigned int color)
{
   // get pixel and ptr
   Uint8* ptr = &m_vecPixels[0] + starty*xres + startx;

   // fill line by line
   for(unsigned int y=0; y<height; y++)
      memset(&ptr[y*xres], color, width);
}

void IndexedImage::Clear(Uint8 index)
{
   if (m_vecPixels.size() > 0)
      memset(&m_vecPixels[0],index,m_vecPixels.size());
}
