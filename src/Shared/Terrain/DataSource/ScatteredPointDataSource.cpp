//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ScatteredPointDataSource.cpp Data source for terrain using ScatteredPointInterpolator
//

// includes
#include "StdAfx.h"
#include "ScatteredPointDataSource.hpp"
#include "Model/DataBlock.hpp"
#include "ScatteredPointInterpolator.hpp"

using namespace Terrain;

ScatteredPointDataSource::ScatteredPointDataSource()
{
   std::vector<Vector3d> vecRefPoints;
   vecRefPoints.push_back(Vector3d(1.0, 1.0, 200.0));
   vecRefPoints.push_back(Vector3d(2.0, 2.0, -100.0));

   vecRefPoints.push_back(Vector3d(3.0, 1.0, 150.0));
   vecRefPoints.push_back(Vector3d(3.0, 3.0, -50.0));

   vecRefPoints.push_back(Vector3d(1.0, 3.0, 100.0));

   m_spInterpolator.reset(new ScatteredPointInterpolator(0.5, vecRefPoints));

   m_spInterpolator->Init();
}

std::shared_ptr<DataBlock> ScatteredPointDataSource::LoadBlock(unsigned int xstart, unsigned int ystart, unsigned int size)
{
   ATLASSERT(true == IsPowerOfTwo(size));

   std::shared_ptr<DataBlock> spDataBlock(new DataBlock(size));
   ElevationData& elevationData = spDataBlock->GetElevationData();

   std::vector<float>& vecElevationData = elevationData.GetElevationData();

   unsigned int xmax = std::min<unsigned int>(xstart + size, 512);
   unsigned int ymax = std::min<unsigned int>(ystart + size, 512);

   LinearTransform<double> tr(0.0, 512.0, 0.0, 4.0);

   for (unsigned int x = xstart; x<=xmax; x++)
   for (unsigned int y = ystart; y<=ymax; y++)
   {
      double dElevation = m_spInterpolator->Height(Vector2d(tr(x), tr(y)));

      size_t uiIndex = elevationData.GetArrayMapper().CoordToIndex(x % size, y % size);
      vecElevationData[uiIndex] = float(dElevation);
   }

   return spDataBlock;
}
