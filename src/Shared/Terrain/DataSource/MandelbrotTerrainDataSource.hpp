//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MandelbrotTerrainDataSource.hpp Data source for terrain using Mandelbrot equation
//
#pragma once

// includes
#include "IDataSource.hpp"
#include "TerrainCommon.hpp"

namespace Terrain
{

/// data source that provides terrain from a mandelbrot set
class TERRAIN_DECLSPEC MandelbrotTerrainDataSource: public IDataSource
{
public:
   /// ctor
   MandelbrotTerrainDataSource() {}
   /// dtor
   virtual ~MandelbrotTerrainDataSource() {}

   /// calculates and returns block
   virtual std::shared_ptr<Terrain::Model::DataBlock> LoadBlock(unsigned int x, unsigned int y, unsigned int size) override;

private:
   /// iterates complex number
   static unsigned int Iterate(double x, double y);
};

} // namespace Terrain
