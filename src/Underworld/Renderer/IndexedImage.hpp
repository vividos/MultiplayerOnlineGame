//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Renderer/IndexedImage.hpp Palette indexed image
//
#pragma once

// includes
#include <vector>

// forward references
class Palette256;

/// palette indexed image
class IndexedImage
{
public:
   /// ctor
   IndexedImage()
      :xres(0),
       yres(0)
   {
   }

   /// dtor
   ~IndexedImage(){}

   /// returns image x resolution
   unsigned int XRes() const { return xres; }
   /// returns image y resolution
   unsigned int YRes() const { return yres; }

   // image manipulation

   /// creates a new bitmap
   void Create(unsigned int width, unsigned int height);

   /// pastes a rectangular area from given image
   void PasteRect(const IndexedImage& fromImage, unsigned int fromx, unsigned int fromy,
      unsigned width, unsigned height, unsigned int destx, unsigned int desty,
      bool transparent = false);

   /// pastes whole given image to position
   void PasteImage(const IndexedImage& fromImage, unsigned int destx,
      unsigned int desty, bool transparent = false)
   {
      PasteRect(fromImage, 0, 0, fromImage.XRes(), fromImage.YRes(), destx, desty, transparent);
   }

   /// fills a specific rectangle with a color
   void FillRect(unsigned int startx, unsigned int starty,
      unsigned int width, unsigned int height, unsigned int color);

   /// clears bitmap with one palette index
   void Clear(Uint8 index = 0);

   /// returns the pixel vector
   std::vector<Uint8>& Pixels(){ return m_vecPixels; }

   /// returns a const pixel vector; const version
   const std::vector<Uint8>& Pixels() const { return m_vecPixels; }

   /// sets palette
   void Palette(std::shared_ptr<Palette256> spPalette){ m_spPalette = spPalette; }

   /// returns palette
   std::shared_ptr<Palette256>& Palette(){ return m_spPalette; };

   /// returns palette; const version
   const std::shared_ptr<Palette256>& Palette() const { return m_spPalette; };

protected:
   /// image resolution
   unsigned int xres, yres;

   /// pixels
   std::vector<Uint8> m_vecPixels;

   /// smart pointer to palette to use
   std::shared_ptr<Palette256> m_spPalette;
};
