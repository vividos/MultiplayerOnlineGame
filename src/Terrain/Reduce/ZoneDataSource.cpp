//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ZoneDataSource.cpp Data source for terrain that splits a block in smaller square zones
//

// includes
#include "StdAfx.h"
#include "ZoneDataSource.hpp"
#include "Model/DataBlock.hpp"
#include "Math.hpp"

using Terrain::Reduce::ZoneDataSource;
using Terrain::Model::DataBlock;
using Terrain::Model::ElevationData;

ZoneDataSource::ZoneDataSource(const DataBlock& baseDataBlock, unsigned int uiZoneSize) throw()
:m_spBaseBlock(new DataBlock(baseDataBlock)),
 m_uiZoneSize(uiZoneSize)
{
   ATLASSERT(IsPowerOfTwo(baseDataBlock.GetElevationData().Size()));
   ATLASSERT(IsPowerOfTwo(uiZoneSize));
   ATLASSERT(baseDataBlock.GetElevationData().Size() >= uiZoneSize);
}

std::shared_ptr<DataBlock> ZoneDataSource::LoadBlock(unsigned int xstart, unsigned int ystart, unsigned int size)
{
   ATLASSERT(true == IsPowerOfTwo(size));

   ElevationData& srcElevationData = m_spBaseBlock->GetElevationData();

   ATLASSERT(m_uiZoneSize == size);
   ATLASSERT(xstart + size <= srcElevationData.Size());
   ATLASSERT(ystart + size <= srcElevationData.Size());

   // prepare sub data block
   std::shared_ptr<DataBlock> spDataBlock(new DataBlock(size));
   ElevationData& destElevationData = spDataBlock->GetElevationData();

   unsigned int xmax = std::min<unsigned int>(xstart + size, srcElevationData.Size());
   unsigned int ymax = std::min<unsigned int>(ystart + size, srcElevationData.Size());

   for (unsigned int x = xstart; x<=xmax; x++)
   for (unsigned int y = ystart; y<=ymax; y++)
   {
      double dHeight = srcElevationData.Height(x, y);
      destElevationData.Height(x - xstart, y - ystart, dHeight);
   }

   return spDataBlock;
}
