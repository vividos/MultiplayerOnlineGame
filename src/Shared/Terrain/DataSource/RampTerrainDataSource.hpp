//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file RampTerrainDataSource.hpp Data source for terrain using a ramp
//
#pragma once

// includes
#include "IDataSource.hpp"
#include "TerrainCommon.hpp"

namespace Terrain
{

/// data source that provides a ramp
class RampTerrainDataSource: public IDataSource
{
public:
   /// ctor
   RampTerrainDataSource(){}
   /// dtor
   virtual ~RampTerrainDataSource() throw() {}

   /// loads block
   virtual std::shared_ptr<Terrain::Model::DataBlock> LoadBlock(unsigned int x, unsigned int y, unsigned int size) override;
};

} // namespace Terrain
