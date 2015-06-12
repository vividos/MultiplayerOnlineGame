//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SineCosineTerrainDataSource.cpp Data source for terrain using sin() and cos()
//

// includes
#include "StdAfx.h"
#include "SineCosineTerrainDataSource.hpp"
#include "Model/DataBlock.hpp"
#include <cmath>

using namespace Terrain;

std::shared_ptr<Terrain::Model::DataBlock> SineCosineTerrainDataSource::LoadBlock(unsigned int xstart, unsigned int ystart, unsigned int size)
{
   ATLASSERT(true == IsPowerOfTwo(size));

   std::shared_ptr<Terrain::Model::DataBlock> spDataBlock(new Terrain::Model::DataBlock(size));
   Terrain::Model::ElevationData& elevationData = spDataBlock->GetElevationData();

   for (unsigned int x = xstart, xmax = xstart + size; x<=xmax; x++)
   for (unsigned int y = ystart, ymax = ystart + size; y<=ymax; y++)
   {
      double dHeightX = 16.0 * cos(x * 0.025 * m_dCosineFactor);
      double dHeightY = 16.0 * sin(y * 0.025 * m_dSineFactor);
      double dHeight = m_dAmplitude * std::max(dHeightX, dHeightY);

      elevationData.Height(x, y, dHeight);
   }

   return spDataBlock;
}
