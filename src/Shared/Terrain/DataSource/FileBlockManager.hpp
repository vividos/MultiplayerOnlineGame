//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file FileBlockManager.hpp file block manager
//
#pragma once

// includes
#include "TerrainCommon.hpp"
#include <vector>

namespace Terrain
{
// forward references
class FileDataBlock;

/// \brief terrain block manager from file system
/// \details manages a square 2D array of blocks with terrain data
class TERRAIN_DECLSPEC FileBlockManager
{
public:
   /// ctor; takes size of side of square 2D block array
   FileBlockManager(unsigned int uiSize = 0);

   /// returns size of side of square 2D block array
   unsigned int Size() const;

   /// creates a new empty block; clears existing block
   void Create(unsigned int xblock, unsigned int yblock);

   /// loads a single block (when not already loaded)
   void Load(unsigned int xblock, unsigned int yblock);

   /// writes a single block to disc
   void Save(unsigned int xblock, unsigned int yblock);

   /// returns block; may be null when not loaded yet
   std::shared_ptr<FileDataBlock> GetBlock(unsigned int xblock, unsigned int yblock);

   /// frees block
   void Free(unsigned int xblock, unsigned int yblock);

private:
   /// check how much blocks are available
   void CheckBlockIndices();

   /// generates block filename
   static CString GenerateFilename(unsigned int uiIndex);

private:
   /// size of side of array
   unsigned int m_uiSize;

   /// block infos
   std::vector<std::shared_ptr<FileDataBlock>> m_vecBlockData;
};

} // namespace Terrain
