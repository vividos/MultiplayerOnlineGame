//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TrianglesCompilerIndex.hpp triangles compiler using an index buffer
//
#pragma once

// includes
#include <vector>
#include "TrianglesCompiler.hpp"

// forward references
class ArrayMapper2D;

namespace Terrain
{
namespace Model
{
class DataBlock;
}
namespace Reduce
{
class IActiveVertexMap;

/// \brief triangles compiler for reduced triangles algorithm, returning indices
class TrianglesCompilerIndex: public TrianglesCompiler
{
public:
   /// ctor
   TrianglesCompilerIndex(IActiveVertexMap& activeVertexMap,
      const ArrayMapper2D& blockMapper, const Model::DataBlock& dataBlock,
      unsigned int xoffset, unsigned int yoffset)
      :TrianglesCompiler(activeVertexMap),
       m_activeVertexMap(activeVertexMap),
       m_blockMapper(blockMapper),
       m_dataBlock(dataBlock),
       m_xoffset(xoffset),
       m_yoffset(yoffset)
   {
   }

   /// returns triangle indices
   const std::vector<unsigned int>& GetIndices() const { return m_vecIndex; }

   /// reduce triangles
   void Reduce();

private:
   /// Add a vertex to the vertex list
   virtual void CompileVertex(unsigned int x, unsigned int y) override;

private:
   /// active vertex map
   IActiveVertexMap& m_activeVertexMap;

   /// data block
   const Model::DataBlock& m_dataBlock;

   /// block to index mapper
   const ArrayMapper2D& m_blockMapper;

   /// indices; used during CompileBlock()
   std::vector<unsigned int> m_vecIndex;

   /// x coordinate offset
   unsigned int m_xoffset;

   /// y coordinate offset
   unsigned int m_yoffset;
};

} // namespace Reduce
} // namespace Terrain
