//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ScatteredPointDataSource.hpp Data source for terrain using ScatteredPointInterpolator
//
#pragma once

// includes
#include "IDataSource.hpp"
#include "TerrainCommon.hpp"

// forward references
class ScatteredPointInterpolator;

namespace Terrain
{

/// data source that provides terrain from scattered point interpolator
class TERRAIN_DECLSPEC ScatteredPointDataSource: public IDataSource
{
public:
   /// ctor
   ScatteredPointDataSource() throw();
   /// dtor
   virtual ~ScatteredPointDataSource() throw() {}

   /// calculates and returns block
   virtual std::shared_ptr<Terrain::Model::DataBlock> LoadBlock(unsigned int x, unsigned int y, unsigned int size) override;

private:
   /// scattered point interpolator
   std::shared_ptr<::ScatteredPointInterpolator> m_spInterpolator;
};

} // namespace Terrain
