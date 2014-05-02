//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file DataBlockManager.hpp Data block manager class
//
#pragma once

// includes
#include "TerrainCommon.hpp"
#include "DataBlockMap.hpp"
#include <functional>

// forward references
class GraphicsTaskManager;

namespace Terrain
{
// forward references
class IDataSource;

namespace Model
{
// forward references
class DataBlock;

/// \brief manages terrain data blocks
/// \details supports loading terrain data blocks from data source using
/// worker thread(s). Each data block has a data block renderer associated
/// (usually the same type for every block). The data block renderer is
/// initialized in the worker thread, too.
class TERRAIN_DECLSPEC DataBlockManager
{
public:
   /// ctor
   DataBlockManager(GraphicsTaskManager& taskManager,
                    std::shared_ptr<IDataSource> spDataSource);

   /// returns if a block is available or is being prepared
   bool IsAvail(unsigned int xblock, unsigned int yblock);

   /// function to call when a block has been loaded
   typedef std::function<void(std::shared_ptr<DataBlock>)> T_fnOnLoadedBlock;

   /// loads a block; calls handler when done
   void AsyncLoadBlock(unsigned int xblock, unsigned int yblock, T_fnOnLoadedBlock fnOnLoadedBlock);

   /// returns terrain block; might return empty block when not already loaded
   std::shared_ptr<DataBlock> GetBlock(unsigned int xblock, unsigned int yblock);

   /// cleans up data blocks, depending on current block coordinates; does not block
   void Cleanup(unsigned int xblock, unsigned int yblock);

   /// returns block size
   static unsigned int BlockSize() throw();

   /// returns height for given coordinate
   double Height(double x, double y) throw();

private:
   /// thread procedure for Prepare() call
   void PrepareBlock(unsigned int xblock, unsigned int yblock, T_fnOnLoadedBlock fnOnLoadedBlock);

   /// thread procedure for Cleanup() call
   void CleanupBlock(unsigned int xblock, unsigned int yblock);

private:
   /// task manager
   GraphicsTaskManager& m_taskManager;

   /// default block to return when requested block isn't loaded yet
   std::shared_ptr<DataBlock> m_spDefaultBlock;

   /// data source
   std::shared_ptr<IDataSource> m_spDataSource;

   /// block map
   DataBlockMap m_blockMap;
};

} // namespace Model
} // namespace Terrain
