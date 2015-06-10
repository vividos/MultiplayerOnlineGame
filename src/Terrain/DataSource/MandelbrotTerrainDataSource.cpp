//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MandelbrotTerrainDataSource.cpp Data source for terrain using Mandelbrot equation
//

// includes
#include "StdAfx.h"
#include "MandelbrotTerrainDataSource.hpp"
#include "Model/DataBlock.hpp"

using namespace Terrain;

std::shared_ptr<Terrain::Model::DataBlock> MandelbrotTerrainDataSource::LoadBlock(unsigned int xstart, unsigned int ystart, unsigned int size)
{
   ATLASSERT(true == IsPowerOfTwo(size));

   std::shared_ptr<Terrain::Model::DataBlock> spDataBlock(new Terrain::Model::DataBlock(size));
   Terrain::Model::ElevationData& elevationData = spDataBlock->GetElevationData();

   unsigned int xmax = std::min<unsigned int>(xstart + size, 512);
   unsigned int ymax = std::min<unsigned int>(ystart + size, 512);

   xstart = xstart % 512;
   ystart = ystart % 512;

   LinearTransform<double> tr(0.0, 512.0, -2.0, 2.0);

   for (unsigned int x = xstart; x<=xmax; x++)
   for (unsigned int y = ystart; y<=ymax; y++)
   {
      unsigned int uiIterations = Iterate(tr(x), tr(y));

      double dElevation = uiIterations / 2.0;

      elevationData.Height(x % size, y % size, dElevation);
   }

   return spDataBlock;
}

unsigned int MandelbrotTerrainDataSource::Iterate(double x, double y) throw()
{
   const unsigned int c_uiMaxIter = 64;
   unsigned int uiIter = 0;

   double re = 0.0, im = 0.0;
   do
   {
      double help = re;
      re = re*re - im*im + x;
      im = 2*help*im + y;

      uiIter++;

   } while(uiIter < c_uiMaxIter && (re*re + im*im) < 4);

   return uiIter;
}
