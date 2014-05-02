//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ImageFileTerrainDataSource.cpp Data source for terrain using image RGB values
//

// includes
#include "StdAfx.h"
#include "ImageFileTerrainDataSource.hpp"
#include "Model/DataBlock.hpp"
#include "Filesystem.hpp"
#include "TgaImageReader.hpp"
#include <ulib/stream/FileStream.hpp>

using namespace Terrain;
using Stream::FileStream;

ImageFileTerrainDataSource::ImageFileTerrainDataSource(LPCTSTR pszTgaFilename)
:m_spReader(new TgaImageReader)
{
   // load tga file
   FileStream fs(pszTgaFilename, FileStream::modeOpen, FileStream::accessRead, FileStream::shareRead);

   m_spReader->Load(fs);

   if (m_spReader->Height() != 512 && m_spReader->Width() != 512)
      throw Exception(CString(_T("tga file not square, and not 512x512: ")) + pszTgaFilename, __FILE__, __LINE__);
}

size_t ImageFileTerrainDataSource::MapCoordToIndex(unsigned int x, unsigned int y)
{
   return (y << 9) | x;
}

std::shared_ptr<DataBlock> ImageFileTerrainDataSource::LoadBlock(unsigned int xstart, unsigned int ystart, unsigned int size)
{
   ATLASSERT(true == IsPowerOfTwo(size));

   std::shared_ptr<DataBlock> spDataBlock(new DataBlock(size));
   ElevationData& elevationData = spDataBlock->GetElevationData();

   std::vector<float>& vecElevationData = elevationData.GetElevationData();

   const std::vector<Color>& vecPixels = m_spReader->Pixels();

   unsigned int xmax = std::min<unsigned int>(xstart + size, 512);
   unsigned int ymax = std::min<unsigned int>(ystart + size, 512);

   for (unsigned int x = xstart; x<xmax; x++)
   for (unsigned int y = ystart; y<ymax; y++)
   {
      const Color& c = vecPixels[MapCoordToIndex(x, y)];

      float fElevation = float(
         (c.m_color[Color::green] << 16) |
         (c.m_color[Color::red] << 8) |
          c.m_color[Color::blue]);

      fElevation /= 2.0*256.0*256.0;
      fElevation -= 16.0;

      size_t uiIndex = elevationData.GetArrayMapper().CoordToIndex(x, y);
      vecElevationData[uiIndex] = fElevation;
   }

   // note: border with x==xmax and border with y==ymax left at 0.0

   return spDataBlock;
}
