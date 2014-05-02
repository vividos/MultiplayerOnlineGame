//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file BlockRenderDataMap.cpp map for data block render data
//

// includes
#include "StdAfx.h"
#include "BlockRenderDataMap.hpp"

using Terrain::View::IBlockRenderData;
using Terrain::View::BlockRenderDataMap;

bool BlockRenderDataMap::IsAvail(unsigned int xblock, unsigned int yblock) const
{
   return m_mapRenderData.find(std::make_pair(xblock, yblock)) != m_mapRenderData.end();
}

void BlockRenderDataMap::Store(unsigned int xblock, unsigned int yblock, std::shared_ptr<IBlockRenderData> spRenderData)
{
   m_mapRenderData[std::make_pair(xblock, yblock)] = spRenderData;
}

std::shared_ptr<IBlockRenderData> BlockRenderDataMap::Get(unsigned int xblock, unsigned int yblock)
{
   return m_mapRenderData[std::make_pair(xblock, yblock)];
}

void BlockRenderDataMap::Delete(unsigned int xblock, unsigned int yblock)
{
   m_mapRenderData.erase(std::make_pair(xblock, yblock));
}
