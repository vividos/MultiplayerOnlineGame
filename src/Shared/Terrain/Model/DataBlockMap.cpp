//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file DataBlockMap.cpp map for terrain data blocks
//

// includes
#include "StdAfx.h"
#include "DataBlockMap.hpp"

using Terrain::Model::DataBlockMap;

bool DataBlockMap::IsLoaded(unsigned int xblock, unsigned int yblock)
{
   MutexLock<RecursiveMutex> lock(m_mtxMapBlocks);

   bool bFound = m_mapBlocks.find(std::make_pair(xblock, yblock)) != m_mapBlocks.end();
   return bFound;
}

bool DataBlockMap::IsPrepared(unsigned int xblock, unsigned int yblock)
{
   if (IsLoaded(xblock, yblock))
      return true;

   // else check the prepare flag
   MutexLock<RecursiveMutex> lock(m_mtxMapBlocks);
   bool bFound = m_setPrepareFlag.find(std::make_pair(xblock, yblock)) != m_setPrepareFlag.end();
   return bFound;
}

void DataBlockMap::SetPrepareFlag(unsigned int xblock, unsigned int yblock, bool bPrepareActive)
{
   MutexLock<RecursiveMutex> lock(m_mtxMapBlocks);
   if (bPrepareActive)
      m_setPrepareFlag.insert(std::make_pair(xblock, yblock));
   else
      m_setPrepareFlag.erase(std::make_pair(xblock, yblock));
}

void DataBlockMap::Store(unsigned int xblock, unsigned int yblock, std::shared_ptr<Terrain::Model::DataBlock> spDataBlock)
{
   MutexLock<RecursiveMutex> lock(m_mtxMapBlocks);

   m_mapBlocks[std::make_pair(xblock, yblock)] = spDataBlock;
}

std::shared_ptr<Terrain::Model::DataBlock> DataBlockMap::Get(unsigned int xblock, unsigned int yblock)
{
   MutexLock<RecursiveMutex> lock(m_mtxMapBlocks);

   std::shared_ptr<DataBlock> spDataBlock = m_mapBlocks[std::make_pair(xblock, yblock)];
   return spDataBlock;
}

void DataBlockMap::Delete(unsigned int xblock, unsigned int yblock)
{
   MutexLock<RecursiveMutex> lock(m_mtxMapBlocks);

   m_mapBlocks.erase(std::make_pair(xblock, yblock));
}
