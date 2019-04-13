//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SineCosineTerrainDataSource.hpp Data source for terrain using sin() and cos()
//
#pragma once

// includes
#include "IDataSource.hpp"
#include "TerrainCommon.hpp"

namespace Terrain
{

/// data source that provides a sine cosine terrain
class TERRAIN_DECLSPEC SineCosineTerrainDataSource: public IDataSource
{
public:
   /// ctor
   SineCosineTerrainDataSource(double dAmplitude = 1.0, double dSineFactor = 3.0, double dCosineFactor = 2.0)
      :m_dAmplitude(dAmplitude),
       m_dSineFactor(dSineFactor),
       m_dCosineFactor(dCosineFactor)
   {
   }

   /// dtor
   virtual ~SineCosineTerrainDataSource() {}

   /// loads block
   virtual std::shared_ptr<Terrain::Model::DataBlock> LoadBlock(unsigned int x, unsigned int y, unsigned int size) override;

private:
   double m_dAmplitude;    ///< amplitude of wave
   double m_dSineFactor;   ///< sine factor
   double m_dCosineFactor; ///< cosine factor
};

} // namespace Terrain
