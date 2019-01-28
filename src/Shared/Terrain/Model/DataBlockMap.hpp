//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file DataBlockMap.hpp map for terrain data blocks
//
#pragma once

// includes
#include <ulib/thread/Mutex.hpp>
#include <map>
#include <set>

namespace Terrain
{
namespace Model
{
// forward references
class DataBlock;

/// map for terrain data blocks
class DataBlockMap
{
public:
   /// returns if a block is already loaded
   bool IsLoaded(unsigned int xblock, unsigned int yblock);

   /// returns if a block is already being flagged for preparing
   bool IsPrepared(unsigned int xblock, unsigned int yblock);

   /// flags block for preparing
   void SetPrepareFlag(unsigned int xblock, unsigned int yblock, bool bPrepareActive);

   /// stores block
   void Store(unsigned int xblock, unsigned int yblock, std::shared_ptr<DataBlock> spDataBlock);

   /// returns block
   std::shared_ptr<DataBlock> Get(unsigned int xblock, unsigned int yblock);

   /// deletes block
   void Delete(unsigned int xblock, unsigned int yblock);

private:
   /// type for block map; maps xblock and yblock coordinates to actual loaded data blocks
   typedef std::map<std::pair<unsigned int, unsigned int>, std::shared_ptr<DataBlock>> T_mapBlocks;

   /// type for prepare flag map
   typedef std::set<std::pair<unsigned int, unsigned int>> T_setPrepareFlag;

   /// mutex to protect m_mapBlocks
   RecursiveMutex m_mtxMapBlocks;

   /// block map
   T_mapBlocks m_mapBlocks;

   /// prepare flag set
   T_setPrepareFlag m_setPrepareFlag;
};

} // namespace Model
} // namespace Terrain
