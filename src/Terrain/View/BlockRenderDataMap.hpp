//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file BlockRenderDataMap.hpp map for data block render data
//
#pragma once

// includes
#include "TerrainCommon.hpp"
#include <ulib/Mutex.hpp>
#include <map>
#include <set>

namespace Terrain
{
namespace View
{
// forward references
class IBlockRenderData;

/// map for terrain data block render data
class TERRAIN_DECLSPEC BlockRenderDataMap
{
public:
   /// returns if render data is available
   bool IsAvail(unsigned int xblock, unsigned int yblock) const;

   /// stores render data
   void Store(unsigned int xblock, unsigned int yblock, std::shared_ptr<IBlockRenderData> spRenderData);

   /// returns render data
   std::shared_ptr<IBlockRenderData> Get(unsigned int xblock, unsigned int yblock);

   /// deletes render data
   void Delete(unsigned int xblock, unsigned int yblock);

private:
   /// type for render data map; maps xblock and yblock coordinates to render data
   typedef std::map<std::pair<unsigned int, unsigned int>, std::shared_ptr<IBlockRenderData>> T_mapRenderData;

   /// render data map
   T_mapRenderData m_mapRenderData;
};

} // namespace View
} // namespace Terrain
