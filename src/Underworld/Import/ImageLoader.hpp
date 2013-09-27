//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/Import/ImageLoader.hpp Image loader
//
#pragma once

// includes
#include "Palette256.hpp"
#include "IndexedImage.hpp"

// forward references
class IFileSystem;

namespace Stream
{
class IStream;
}

namespace Import
{

/// image loader class
class ImageLoader
{
public:
   /// ctor
   ImageLoader(IFileSystem& fileSystem)
      :m_fileSystem(fileSystem)
   {
   }

   /// loads 8 main palettes
   void LoadPalettes(LPCTSTR pszAllPalettesName,
      std::array<std::shared_ptr<Palette256>, 8>& aAllPalettes);

   /// loads all 32 auxiliary palettes with 16 indices each
   void LoadAuxPalettes(LPCTSTR pszAuxPaletteName,
      Uint8 aAllAuxPalettes[32][16]);

   /// loads list of images from .gr file
   void LoadImageGrList(std::vector<IndexedImage>& vecImages,
      LPCTSTR pszImageName,
      Uint8 aAuxPalettes[32][16],
      unsigned int uiImageFrom = 0,
      unsigned int uiImageTo = 0, // 0 means "load all"
      bool bPanelsGrFix = false);

private:
   /// loads *.gr image into pixels array
   static void LoadImageGrImpl(IndexedImage& image,
      std::shared_ptr<Stream::IStream> spFile,
      Uint8 aAuxPalettes[32][16],
      bool bPanelsGrFix = false);

   /// decodes RLE encoded image
   static void DecodeRLEImage(std::shared_ptr<Stream::IStream> spFile, Uint8* pixels, unsigned int bits,
      unsigned int datalen, unsigned int maxpix, unsigned char *aAuxPalettes,
      unsigned int padding = 0, unsigned int linewidth = 0);

private:
   /// file system
   IFileSystem& m_fileSystem;
};

} // namespace Import
