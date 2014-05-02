//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ReduceAlgorithm.hpp Terrain triangles reduce algorithm
//
#pragma once

// includes
#include "Vector3d.hpp"
#include "Model/DataBlock.hpp"
#include <vector>

namespace Terrain
{
namespace Reduce
{
// forward references
class IActiveVertexMap;

/// \brief terrain level of detail triangle reduce algorithm base class
class ReduceAlgorithmBase
{
public:
   /// ctor
   ReduceAlgorithmBase(const Model::DataBlock& dataBlock)
      :m_uiSize(dataBlock.GetElevationData().Size()),
       m_elevationData(dataBlock.GetElevationData())
   {
   }

   /// reduces triangles for all blocks
   void Reduce(IActiveVertexMap& activeVertexMap);

protected:
   /// returns boundary value
   /// \see http://bits.stephan-brumme.com/lowestBitSet.html
   unsigned int GetBoundary(unsigned int n) const throw()
   {
      return n == 0 ? m_uiSize : (n & -int(n));
   }

   /// returns map size
   unsigned int GetSize() const throw() { return m_uiSize; }

   /// examines quad and reduces triangles
   virtual void DoQuad(IActiveVertexMap& activeVertexMap, unsigned int x1, unsigned int y1, unsigned int size) = 0;

   /// activates an edge point
   void ActivatePoint(IActiveVertexMap& activeVertexMap, int x, int y);

   /// map point to elevation data
   Vector3d MapPoint(unsigned int x, unsigned  int y) const throw();

private:
   /// block size
   unsigned int m_uiSize;

   /// elevation data
   const Model::ElevationData& m_elevationData;
};

/// \brief terrain level of detail triangle reduce algorithm
/// \details uses Shamus Young's reduce algorithm to determine active (to be
/// rendered) and inactive vertices in the terrain mesh. An instance of the
/// ReducedTriangleBlockRenderData class renders the mesh then.
class ReduceAlgorithmCameraDistance: public ReduceAlgorithmBase
{
public:
   /// ctor
   ReduceAlgorithmCameraDistance(const Model::DataBlock& dataBlock)
      :ReduceAlgorithmBase(dataBlock)
   {
   }
   /// dtor
   virtual ~ReduceAlgorithmCameraDistance() throw() {}

   /// reduces triangles for all blocks, based on camera position
   void Reduce(IActiveVertexMap& activeVertexMap, const Vector3d& vCamera);

   /// examines quad and reduces triangles
   virtual void DoQuad(IActiveVertexMap& activeVertexMap, unsigned int x1, unsigned int y1, unsigned int size) override;

protected:
   double CalcDelta(unsigned int x1, unsigned int y1, unsigned int size) throw();

private:
   /// maps distance from position
   double MapDistance(unsigned int x, unsigned int y);

private:
   /// camera position
   Vector3d m_vCamera;
};

/// \brief terrain level of detail triangle reduce algorithm
class ReduceAlgorithmLevelOfDetail: public ReduceAlgorithmCameraDistance
{
public:
   /// ctor
   ReduceAlgorithmLevelOfDetail(const Model::DataBlock& dataBlock)
      :ReduceAlgorithmCameraDistance(dataBlock),
       m_uiLevel(0)
   {
   }
   virtual ~ReduceAlgorithmLevelOfDetail() throw() {}

   /// reduces triangles for all blocks, based on level of detail
   void Reduce(IActiveVertexMap& activeVertexMap, unsigned int uiLevel);

   /// examines quad and reduces triangles
   virtual void DoQuad(IActiveVertexMap& activeVertexMap, unsigned int x1, unsigned int y1, unsigned int size) override;

private:
   /// level of detail
   unsigned int m_uiLevel;
};

} // namespace Reduce
} // namespace Terrain
