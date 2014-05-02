//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file DataBlockManager.cpp Data block manager class
//

// includes
#include "StdAfx.h"
#include "DataBlockManager.hpp"
#include "GraphicsTaskManager.hpp"
#include "DataBlock.hpp"
#include "DataSource/IDataSource.hpp"

using Terrain::Model::DataBlockManager;

/// size of a data block
const unsigned int c_uiBlockSize = 512;

DataBlockManager::DataBlockManager(GraphicsTaskManager& taskManager,
                                   std::shared_ptr<Terrain::IDataSource> spDataSource)
:m_taskManager(taskManager),
 m_spDefaultBlock(new Terrain::Model::DataBlock(c_uiBlockSize)),
 m_spDataSource(spDataSource)
{
   ATLASSERT(spDataSource != NULL);
}

bool DataBlockManager::IsAvail(unsigned int xblock, unsigned int yblock)
{
   // already in map, or being prepared?
   return m_blockMap.IsPrepared(xblock, yblock);
}

void DataBlockManager::AsyncLoadBlock(unsigned int xblock, unsigned int yblock,
                                      T_fnOnLoadedBlock fnOnLoadedBlock)
{
   if (m_blockMap.IsPrepared(xblock, yblock))
      return; // already in map, or being prepared

   ATLTRACE(_T("Queue preparing block %u, %u\n"), xblock, yblock);

   m_blockMap.SetPrepareFlag(xblock, yblock, true);

   m_taskManager.BackgroundTaskGroup().Add(
      std::bind(&DataBlockManager::PrepareBlock, this, xblock, yblock, fnOnLoadedBlock));
}

std::shared_ptr<Terrain::Model::DataBlock> DataBlockManager::GetBlock(unsigned int xblock, unsigned int yblock)
{
   if (!m_blockMap.IsLoaded(xblock, yblock))
      return m_spDefaultBlock; // not loaded yet

   return m_blockMap.Get(xblock, yblock);
}

void DataBlockManager::Cleanup(unsigned int xblock, unsigned int yblock)
{
   ATLTRACE(_T("Queue cleanup block %u, %u\n"), xblock, yblock);

   m_taskManager.BackgroundTaskGroup().Add(
      std::bind(&DataBlockManager::CleanupBlock, this, xblock, yblock));
}

unsigned int DataBlockManager::BlockSize() throw()
{
   return c_uiBlockSize;
}

double DataBlockManager::Height(double x, double y) throw()
{
   unsigned int xblock = unsigned(x + 1.0) / c_uiBlockSize;
   unsigned int yblock = unsigned(y + 1.0) / c_uiBlockSize;

   if (!IsAvail(xblock, yblock))
      return 0.0;

   std::shared_ptr<DataBlock> spDataBlock = GetBlock(xblock, yblock);
   if (spDataBlock == NULL)
      return 0.0;

   x = fmod(x, c_uiBlockSize);
   y = fmod(y, c_uiBlockSize);

   return spDataBlock->HeightInterpolate(x, y);
}

/// \note runs in worker thread
void DataBlockManager::PrepareBlock(unsigned int xblock, unsigned int yblock, T_fnOnLoadedBlock fnOnLoadedBlock)
{
   ATLTRACE(_T("Prepare block %u, %u\n"), xblock, yblock);

   // load the block
   std::shared_ptr<Terrain::Model::DataBlock> spDataBlock =
      m_spDataSource->LoadBlock(xblock*c_uiBlockSize, yblock*c_uiBlockSize, c_uiBlockSize);

   m_blockMap.Store(xblock, yblock, spDataBlock);

   m_blockMap.SetPrepareFlag(xblock, yblock, false);

   if (fnOnLoadedBlock != NULL)
      fnOnLoadedBlock(spDataBlock);
}

/// \note runs in worker thread
void DataBlockManager::CleanupBlock(unsigned int xblock, unsigned int yblock)
{
   ATLTRACE(_T("Cleanup block %u, %u\n"), xblock, yblock);

   m_blockMap.Delete(xblock, yblock);
}
