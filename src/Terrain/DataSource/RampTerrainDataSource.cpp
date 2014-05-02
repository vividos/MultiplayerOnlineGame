//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file RampTerrainDataSource.cpp Data source for terrain using a ramp
//

// includes
#include "StdAfx.h"
#include "RampTerrainDataSource.hpp"
#include "Model/DataBlock.hpp"

using namespace Terrain;

std::shared_ptr<Terrain::Model::DataBlock> RampTerrainDataSource::LoadBlock(unsigned int xstart, unsigned int ystart, unsigned int size)
{
   ATLASSERT(true == IsPowerOfTwo(size));

   std::shared_ptr<Terrain::Model::DataBlock> spDataBlock(new Terrain::Model::DataBlock(size));
   Terrain::Model::ElevationData& elevationData = spDataBlock->GetElevationData();

   LinearTransform<double> tr(72, 512, 0, 128.0);

   for (unsigned int x = xstart, xmax = xstart + size; x<=xmax; x++)
   for (unsigned int y = ystart, ymax = ystart + size; y<=ymax; y++)
   {
      double dElevation = 0.0;
      if (x >= 72)
         //dElevation = ((x-72)/256.0)*128.0;
         dElevation = tr(x);

      elevationData.Height(x, y, dElevation);
   }

   return spDataBlock;
}
