//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file DataBlock.hpp Terrain height data block
//
#pragma once

// includes
#include "ElevationData.hpp"

namespace Terrain
{
namespace Model
{

/// single terrain data block
class DataBlock
{
public:
   /// ctor
   /// \param uiSize size of (square) data block; must be power of two
   DataBlock(unsigned int uiSize);

   // get methods

   /// returns height at given coordinates inside the block
   double HeightInterpolate(double x, double y) const throw();

   /// returns elevation data
   const ElevationData& GetElevationData() const throw() { return m_elevationData; }

   /// returns elevation data; non-const version
   ElevationData& GetElevationData() throw() { return m_elevationData; }

private:
   /// elevation data
   ElevationData m_elevationData;
};

} // namespace Model
} // namespace Terrain
