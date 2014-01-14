//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Import/TextureLoader.cpp Texture loader
//

// includes
#include "stdafx.h"
#include "TextureLoader.hpp"
#include "IFileSystem.hpp"
#include <ulib/stream/IStream.hpp>
#include <ulib/stream/EndianAwareFilter.hpp>

void Import::TextureLoader::Load(std::vector<IndexedImage>& vecImages,
   unsigned int startidx, LPCTSTR pszTextureFilename,
   std::shared_ptr<Palette256> spPalette)
{
   std::shared_ptr<Stream::IStream> spFile =
      m_fileSystem.OpenFile(pszTextureFilename, true);

   // get file length
   ULONGLONG ullFileLength = spFile->Length();

   // get header values
   Uint8 val = spFile->ReadByte(); // always 2 (.tr)
   ATLVERIFY(val == 2);
   unsigned int xyres = spFile->ReadByte(); // x and y resolution (square textures)

   Stream::EndianAwareFilter filter(*spFile);
   Uint16 entries = filter.Read16LE();

   // reserve needed entries
   if (startidx+entries > vecImages.size())
      vecImages.resize(startidx + entries);

   // read in all offsets
   std::vector<Uint32> vecOffsets(entries);
   for (unsigned int i=0; i<entries; i++)
      vecOffsets[i] = filter.Read32LE();

   // read in all textures
   for (unsigned int tex=0; tex<entries; tex++)
   {
      if (vecOffsets[tex] >= ullFileLength)
         continue;

      // seek to texture entry
      spFile->Seek(vecOffsets[tex], Stream::IStream::seekBegin);

      // alloc memory for texture

      // create new image
      IndexedImage& textureImage = vecImages[startidx+tex];

      textureImage.Create(xyres, xyres);
      textureImage.Palette(spPalette);

      Uint8* pPixels = &textureImage.Pixels()[0];
      DWORD dwBytesRead = 0;
      spFile->Read(pPixels, xyres*xyres, dwBytesRead);
   }
}
