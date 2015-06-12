//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file FileBlockManager.cpp file block manager
//

// includes
#include "StdAfx.h"
#include "FileBlockManager.hpp"
#include "Filesystem.hpp"
#include "Path.hpp"
#include "ArrayMapper2D.hpp"
#include "FileDataBlock.hpp"
#include <ulib/stream/FileStream.hpp>

using Terrain::FileDataBlock;
using Terrain::FileBlockManager;

/// format string for block filename
LPCTSTR c_pszBlockFormatString = _T("block%04x.bin");

/// folder in data dir with terrain blocks
LPCTSTR c_pszTerrainFolder = _T("terrain");

FileBlockManager::FileBlockManager(unsigned int uiSize)
:m_uiSize(uiSize)
{
   if (uiSize == 0)
      CheckBlockIndices();

   m_vecBlockData.resize(m_uiSize*m_uiSize);
}

unsigned int FileBlockManager::Size() const throw()
{
   return m_uiSize;
}

void FileBlockManager::Create(unsigned int xblock, unsigned int yblock)
{
   ArrayMapper2D mapper(m_uiSize, m_uiSize);

   ATLASSERT(mapper.IsInRange(xblock, yblock));

   std::shared_ptr<FileDataBlock>& spDataBlock = m_vecBlockData[mapper.CoordToIndex(xblock, yblock)];

   spDataBlock.reset(new FileDataBlock);
}

void FileBlockManager::Load(unsigned int xblock, unsigned int yblock)
{
   ArrayMapper2D mapper(m_uiSize, m_uiSize);

   ATLASSERT(mapper.IsInRange(xblock, yblock));

   std::shared_ptr<FileDataBlock>& spDataBlock = m_vecBlockData[mapper.CoordToIndex(xblock, yblock)];

   if (spDataBlock != NULL)
      return;

   spDataBlock.reset(new FileDataBlock);

   // load block
   CString cszFilename = GenerateFilename(mapper.CoordToIndex(xblock, yblock));

   Stream::FileStream fs(cszFilename,
      Stream::FileStream::modeOpen,
      Stream::FileStream::accessRead,
      Stream::FileStream::shareRead);

   spDataBlock->Load(fs);
}

void FileBlockManager::Save(unsigned int xblock, unsigned int yblock)
{
   ArrayMapper2D mapper(m_uiSize, m_uiSize);

   ATLASSERT(mapper.IsInRange(xblock, yblock));

   std::shared_ptr<FileDataBlock> spDataBlock = m_vecBlockData[mapper.CoordToIndex(xblock, yblock)];

   ATLASSERT(spDataBlock != NULL);

   // save block
   CString cszFilename = GenerateFilename(mapper.CoordToIndex(xblock, yblock));

   Stream::FileStream fs(cszFilename,
      Stream::FileStream::modeCreate,
      Stream::FileStream::accessWrite,
      Stream::FileStream::shareRead);

   spDataBlock->Save(fs);
}

std::shared_ptr<FileDataBlock> FileBlockManager::GetBlock(unsigned int xblock, unsigned int yblock)
{
   ArrayMapper2D mapper(m_uiSize, m_uiSize);

   ATLASSERT(mapper.IsInRange(xblock, yblock));

   return m_vecBlockData[mapper.CoordToIndex(xblock, yblock)];
}

void FileBlockManager::Free(unsigned int xblock, unsigned int yblock)
{
   ArrayMapper2D mapper(m_uiSize, m_uiSize);

   ATLASSERT(mapper.IsInRange(xblock, yblock));

   m_vecBlockData[mapper.CoordToIndex(xblock, yblock)].reset();
}

void FileBlockManager::CheckBlockIndices()
{
   // check how much block files are there; must be a square number
   Path basePath = Path::Combine(Filesystem().BaseFolder(), c_pszTerrainFolder);

   CString cszFilename;
   unsigned int uiMaxBlockFileIndex = 0;
   for (unsigned int ui=0; ui<1024; ui++)
   {
      cszFilename.Format(c_pszBlockFormatString, ui);
      if (!basePath.Combine(cszFilename).FileExists())
      {
         uiMaxBlockFileIndex = ui;
         break;
      }
   }

   if (!IsPowerOfTwo(uiMaxBlockFileIndex))
      throw Exception(_T("didn't find enough terrain blocks for square block array"), __FILE__, __LINE__);

   m_uiSize = unsigned(std::sqrt(double(uiMaxBlockFileIndex)));
}

CString FileBlockManager::GenerateFilename(unsigned int uiIndex)
{
   CString cszFilename;
   cszFilename.Format(c_pszBlockFormatString, uiIndex);

   Path basePath = Path::Combine(Filesystem().BaseFolder(), c_pszTerrainFolder);
   return basePath.Combine(cszFilename);
}
