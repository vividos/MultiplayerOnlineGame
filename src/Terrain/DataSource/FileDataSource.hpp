//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file FileDataSource.hpp Data source for terrain loading from files
//
#pragma once

// includes
#include "TerrainCommon.hpp"
#include "IDataSource.hpp"
#include "FileBlockManager.hpp"
#include <vector>

namespace Terrain
{

/// \brief file based terrain data source
class TERRAIN_DECLSPEC FileDataSource: public IDataSource
{
public:
   /// default ctor
   FileDataSource() throw();

   /// ctor with square block size
   /// \param uiSize size of side of square block array
   FileDataSource(unsigned int uiSize) throw();

   /// dtor
   virtual ~FileDataSource() throw() {}

   /// loads block
   virtual std::shared_ptr<Terrain::Model::DataBlock> LoadBlock(unsigned int x, unsigned int y, unsigned int size) override;

   /// returns block manager
   FileBlockManager& GetBlockManager() throw() { return m_blockManager; }

private:
   /// block manager
   FileBlockManager m_blockManager;
};

} // namespace Terrain
