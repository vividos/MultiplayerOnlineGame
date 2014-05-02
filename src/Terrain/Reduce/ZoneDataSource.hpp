//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ZoneDataSource.hpp Data source for terrain that splits a block in smaller square zones
//
#pragma once

// includes
#include "DataSource/IDataSource.hpp"
#include "TerrainCommon.hpp"

namespace Terrain
{
namespace Reduce
{

/// \brief returns terrain data blocks from a larger terrain data source
/// \details returns data blocks that also contain edge infos
class TERRAIN_DECLSPEC ZoneDataSource: public IDataSource
{
public:
   /// ctor
   ZoneDataSource(const Model::DataBlock& baseDataBlock, unsigned int uiZoneSize) throw();
   /// dtor
   virtual ~ZoneDataSource() throw() {}

   /// returns block
   virtual std::shared_ptr<Terrain::Model::DataBlock> LoadBlock(unsigned int x, unsigned int y, unsigned int size) override;

private:
   /// base block
   std::shared_ptr<Model::DataBlock> m_spBaseBlock;

   /// zone size of subdivided block
   unsigned int m_uiZoneSize;
};

} // namespace Reduce
} // namespace Terrain
