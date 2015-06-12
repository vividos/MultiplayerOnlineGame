//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ImageFileTerrainDataSource.hpp Data source for terrain using image RGB values
//
#pragma once

// includes
#include "IDataSource.hpp"
#include "TerrainCommon.hpp"

// forward references
class TgaImageReader;

namespace Terrain
{

/// image file terrain data source
class TERRAIN_DECLSPEC ImageFileTerrainDataSource: public IDataSource
{
public:
   /// ctor; takes filename of image file
   ImageFileTerrainDataSource(LPCTSTR pszImageFilename);
   /// dtor
   virtual ~ImageFileTerrainDataSource() throw() {}

   /// loads block
   virtual std::shared_ptr<Terrain::Model::DataBlock> LoadBlock(unsigned int x, unsigned int y, unsigned int size) override;

private:
   /// maps coordinates to index in image file color buffer
   static size_t MapCoordToIndex(unsigned int x, unsigned int y) throw();

private:
   /// file image reader
   //std::shared_ptr<TgaImageReader> m_spReader;
};


/// data source that provides terrain from a mandelbrot set
class TERRAIN_DECLSPEC MandelbrotTerrainDataSource: public IDataSource
{
public:
   /// ctor
   MandelbrotTerrainDataSource() throw() {}
   /// dtor
   virtual ~MandelbrotTerrainDataSource() throw() {}

   /// calculates and returns block
   virtual std::shared_ptr<DataBlock> LoadBlock(unsigned int x, unsigned int y, unsigned int size) override;

private:
   /// iterates complex number
   static unsigned int Iterate(double x, double y) throw();
};


#if 0
/// data source that provides terrain from scattered point interpolator
class TERRAIN_DECLSPEC ScatteredPointDataSource: public IDataSource
{
public:
   /// ctor
   ScatteredPointDataSource() throw();
   /// dtor
   virtual ~ScatteredPointDataSource() throw() {}

   /// calculates and returns block
   virtual std::shared_ptr<DataBlock> LoadBlock(unsigned int x, unsigned int y, unsigned int size) override;

private:
   std::shared_ptr<::ScatteredPointInterpolator> m_spInterpolator;
};
#endif


/// \brief returns terrain data blocks from a larger terrain data source
/// \details returns data blocks that also contain edge infos
class TERRAIN_DECLSPEC ZoneDataSource: public IDataSource
{
public:
   /// ctor
   ZoneDataSource(const DataBlock& baseDataBlock, unsigned int uiZoneSize) throw();
   /// dtor
   virtual ~ZoneDataSource() throw() {}

   /// returns block
   virtual std::shared_ptr<DataBlock> LoadBlock(unsigned int x, unsigned int y, unsigned int size) override;

private:
   /// base block
   std::shared_ptr<DataBlock> m_spBaseBlock;

   /// zone size of subdivided block
   unsigned int m_uiZoneSize;
};

} // namespace Terrain
