//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/Renderer/TextureAtlas.cpp Texture atlas
//

// includes
#include "stdafx.h"
#include "TextureAtlas.hpp"
#include "ImageLoader.hpp"
#include "TextureLoader.hpp"
#include "TexCoord2f.hpp"
#include "Texture.hpp"
#include "Bitmap.hpp"
//#include "BitmapImageWriter.hpp"

void TextureAtlas::Load(IFileSystem& fileSystem)
{
   std::array<std::shared_ptr<Palette256>, 8> aAllPalettes;

   Import::ImageLoader imageLoader(fileSystem);
   imageLoader.LoadPalettes(_T("data\\pals.dat"), aAllPalettes);

   Import::TextureLoader textureLoader(fileSystem);

   std::vector<IndexedImage> vecWallTextures, vecFloorTextures;

   textureLoader.Load(vecWallTextures, 0, _T("data\\dw64.tr"), aAllPalettes[0]);
   textureLoader.Load(vecFloorTextures, 0, _T("data\\df32.tr"), aAllPalettes[0]);

   // prepare a big image with all textures
   IndexedImage bigImage;
   bigImage.Create(1024, 1024);
   bigImage.Palette(aAllPalettes[0]);

   // copy all floor textures
   for (size_t i=0, iMax=vecFloorTextures.size(); i<iMax; i++)
   {
      const IndexedImage& src = vecFloorTextures[i];

      const unsigned int xyres = src.XRes();

      unsigned int xpos = i * (xyres + 4); // every res + 4 pixel is a new image
      unsigned int ypos = 0;

      PasteImageWithBorders(src, bigImage, xpos, ypos);
   }

   // copy all wall textures
   for (size_t i=0, iMax=vecWallTextures.size(); i<iMax; i++)
   {
      const IndexedImage& src = vecWallTextures[i];

      const unsigned int xyres = src.XRes();

      unsigned int xpos = (i % 15) * (xyres + 4); // every res + 4 pixel is a new image
      unsigned int ypos = (32 + 8) + (i / 15) * (xyres + 4); // a line holds 15 images each

      PasteImageWithBorders(src, bigImage, xpos, ypos);
   }

   // load all objects
   Uint8 aAuxPalettes[32][16] = {0};
   imageLoader.LoadAuxPalettes(_T("data\\allpals.dat"), aAuxPalettes);

   std::vector<IndexedImage> vecImages;
   imageLoader.LoadImageGrList(vecImages,
      _T("data\\objects.gr"),
      aAuxPalettes, 0, 0, false);

   //std::for_each(vecImages.begin(), vecImages.end(), [&](const IndexedImage& src)
   for (size_t i=0, iMax=vecImages.size(); i<iMax; i++)
   {
      const IndexedImage& src = vecImages[i];

      if (src.XRes() != 16 || src.YRes() != 16)
         continue;

      const unsigned int xyres = src.XRes();
      const unsigned int yoffset = 320;
      const unsigned int perline = 60;

      unsigned int xpos = (i % perline) * (xyres + 1); // every res + 1 pixel is a new image
      unsigned int ypos = yoffset + (i / perline) * (xyres + 1); // a line holds 60 images each

      bigImage.FillRect(xpos, ypos, xyres+1, xyres+1, 0);
      bigImage.PasteImage(src, xpos, ypos, true);
   }

   // load heads.gr
   std::vector<IndexedImage> vecHeadsImages;
   imageLoader.LoadImageGrList(vecHeadsImages,
      _T("data\\heads.gr"),
      aAuxPalettes, 0, 0, false);

   for (size_t i=0, iMax=vecHeadsImages.size(); i<iMax; i++)
   {
      const IndexedImage& src = vecHeadsImages[i];

      if (src.XRes() != 34 || src.YRes() != 34)
         continue;

      const unsigned int xyres = src.XRes();
      const unsigned int yoffset = 320 + 8 * (16+1);

      unsigned int xpos = i * (xyres + 1); // every res + 1 pixel is a new image
      unsigned int ypos = yoffset; // all suite in one line

      bigImage.PasteImage(src, xpos, ypos, true);
   }


   // load genhead.gr
   std::vector<IndexedImage> vecGenHeadsImages;
   imageLoader.LoadImageGrList(vecGenHeadsImages,
      _T("data\\genhead.gr"),
      aAuxPalettes, 0, 0, false);

   for (size_t i=0, iMax=vecGenHeadsImages.size(); i<iMax; i++)
   {
      const IndexedImage& src = vecGenHeadsImages[i];

      if (src.XRes() != 34 || src.YRes() != 34)
         continue;

      const unsigned int xyres = src.XRes();
      const unsigned int yoffset = 496;
      const unsigned int perline = 29;

      unsigned int xpos = (i % perline) * (xyres + 1); // every res + 1 pixel is a new image
      unsigned int ypos = yoffset + (i / perline) * (xyres + 1);

      bigImage.PasteImage(src, xpos, ypos, true);
   }

   m_spBitmap.reset(new Bitmap(bigImage.XRes(), bigImage.YRes()));
   GenerateBitmap(bigImage, *m_spBitmap);

   //BitmapImageWriter writer(*m_spBitmap);
   //writer.Write(_T("d:\\Projekte\\TextureAtlas.bmp"));
}

void TextureAtlas::Generate()
{
   ATLASSERT(m_spBitmap != nullptr); // must be called after calling Load()

   m_spTexture.reset(new Texture());

   m_spTexture->Generate();
   m_spTexture->Bind();
   m_spTexture->Upload(*m_spBitmap, false); // no mipmap

   m_spBitmap.reset(); // reclaim memory
}

void TextureAtlas::GetUV(Uint16 uiPos, TexCoord2f& coordMin, TexCoord2f& coordMax)
{
   unsigned int xyres = 2;
   unsigned int xpos = 0;
   unsigned int ypos = 0;

   if (uiPos >= Underworld::c_uiStockTexturesWall &&
       uiPos < Underworld::c_uiStockTexturesWall + 48)
   {
      unsigned int i = uiPos - Underworld::c_uiStockTexturesWall;

      xyres = 64;
      xpos = (i % 15) * (xyres + 4); // every res + 4 pixel is a new image
      ypos = (32 + 8) + (i / 15) * (xyres + 4); // a line holds 15 images each
   }
   else
   if (uiPos >= Underworld::c_uiStockTexturesFloor &&
       uiPos < Underworld::c_uiStockTexturesFloor + 10)
   {
      unsigned int i = uiPos - Underworld::c_uiStockTexturesFloor;

      xyres = 32;
      xpos = i * (xyres + 4); // every res + 4 pixel is a new image
      ypos = 0;
   }
   else
      ATLASSERT(false); // invalid texture

   coordMin = TexCoord2f(xpos / 1024.0f, ypos / 1024.0f);
   coordMax = TexCoord2f((xpos + xyres) / 1024.0f, (ypos + xyres) / 1024.0f);
}

void TextureAtlas::PasteImageWithBorders(const IndexedImage& src, IndexedImage& dest, unsigned int xpos, unsigned int ypos)
{
   const unsigned int xyres = src.XRes();

   // dest image must have space for border on the right side
   ATLASSERT(xpos + src.XRes() + 1 < dest.XRes());

   // image
   dest.PasteImage(src, xpos, ypos, false);

   // copy right border to the left of image
   unsigned int destx = xpos == 0 ? dest.XRes()-1 : xpos-1;
   dest.PasteRect(src, xyres-1, 0, 1, xyres, destx, ypos, false);

   // copy left border to the right of image
   dest.PasteRect(src, 0, 0, 1, xyres, xpos + xyres, ypos, false);

   // copy top border to the bottom of image
   dest.PasteRect(src, 0, 0, xyres, 1, xpos, ypos + xyres, false);

   // copy bottom border to the top (actually, the bottom of the big texture)
   unsigned int desty = ypos == 0 ? dest.YRes()-1 : ypos-1;
   dest.PasteRect(src, 0, xyres-1, xyres, 1, xpos, desty, false);
}

void TextureAtlas::GenerateBitmap(const IndexedImage& src, Bitmap& dest)
{
   ATLASSERT(dest.XRes() == src.XRes() && dest.YRes() == src.YRes());

   const Palette256& pal = *src.Palette();

   const Uint8* pSrc = &src.Pixels()[0];
   Color* pDest = dest.Data();

   // since IndexedImage and Bitmap use the same way to store pixels (row-first), we
   // can just iterate over the whole size of the image
   for (size_t i=0, iMax = src.XRes() * src.YRes(); i<iMax; i++)
   {
      Uint8 uiIndex = pSrc[i];
      const Uint8* srcColor = &pal.Get()[uiIndex * 4];
      pDest[i] = Color(srcColor);
   }
}
