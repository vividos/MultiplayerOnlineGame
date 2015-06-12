//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ElevationData.hpp Elevation data for terrain
//
#pragma once

// includes
#include "ArrayMapper2D.hpp"
#include "Vector3.hpp"
#include <vector>

namespace Terrain
{
namespace Model
{

/// \brief elevation data for terrain data block
/// \details note that the actual amount of stored points in the elevation data
/// is (size+1)^2
class ElevationData
{
public:
   /// ctor
   /// \param uiSize size of (square) data block; must be power of two
   ElevationData(unsigned int uiSize) throw();

   // set methods

   /// sets height at given integer coordinate
   void Height(unsigned int x, unsigned int y, double dHeight)
   {
      ATLASSERT(x <= m_uiSize && y <= m_uiSize);
      m_vecElevation[m_arrayMapper.CoordToIndex(x,y)] = float(dHeight);
   }

   // get methods

   /// returns size of (square) data block
   unsigned int Size() const throw() { return m_uiSize; }

   /// returns height at given integer coordinate
   double Height(unsigned int x, unsigned int y) const
   {
      ATLASSERT(x <= m_uiSize && y <= m_uiSize);
      return m_vecElevation[m_arrayMapper.CoordToIndex(x,y)];
   }

   /// returns vertex at given integer coordinate
   Vector3d Vertex(unsigned int x, unsigned int y) const
   {
      return Vector3d(x, Height(x,y), y);
   }

   /// returns height at given point; interpolates between points
   double InterpolatedHeight(double x, double y) const;

   /// returns height array
   std::vector<float>& RawData() throw() { return m_vecElevation; }

   /// returns height array; const version
   const std::vector<float>& RawData() const throw() { return m_vecElevation; }

   /// returns vertex by index
   Vector3d VertexByIndex(size_t index) const throw()
   {
      unsigned int x, y;
      m_arrayMapper.IndexToCoord(index, x, y);
      return Vector3d(x, Height(x,y), y);
   }

   /// returns array mapper
   const ArrayMapper2D& ArrayMapper() const throw() { return m_arrayMapper; }

private:
   /// elevation data array
   std::vector<float> m_vecElevation;

   /// size of block
   unsigned int m_uiSize;

   /// array mapper
   ArrayMapper2D m_arrayMapper;
};

} // namespace Model
} // namespace Terrain
