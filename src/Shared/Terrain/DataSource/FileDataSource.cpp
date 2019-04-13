//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file FileDataSource.cpp Data source for terrain loading from files
//

// includes
#include "StdAfx.h"
#include "FileDataSource.hpp"
#include "FileDataBlock.hpp"

using Terrain::FileDataSource;

FileDataSource::FileDataSource()
{
}

FileDataSource::FileDataSource(unsigned int uiSize)
:m_blockManager(uiSize)
{
}

std::shared_ptr<Terrain::Model::DataBlock> FileDataSource::LoadBlock(unsigned int x, unsigned int y, unsigned int uiSize)
{
   ATLASSERT(uiSize == Terrain::c_uiFileDataBlockSize);

   unsigned int uiBlockX = x / uiSize;
   unsigned int uiBlockY = y / uiSize;

   if (uiBlockX >= m_blockManager.Size() ||
       uiBlockY >= m_blockManager.Size())
       throw Exception(_T("file data source block out of range"), __FILE__, __LINE__);

   m_blockManager.Load(uiBlockX, uiBlockY);

   return m_blockManager.GetBlock(uiBlockX, uiBlockY);
}
