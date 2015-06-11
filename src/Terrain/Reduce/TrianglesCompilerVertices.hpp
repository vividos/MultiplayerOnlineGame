//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TrianglesCompilerVertices.hpp triangles compiler using vertex buffer object
//
#pragma once

// includes
#include "TrianglesCompiler.hpp"
#include "TexturedVertexBuffer.hpp"
#include "Vector3.hpp"

namespace Terrain
{
namespace Model
{
class DataBlock;
}
namespace Reduce
{

class TrianglesCompilerVertices: public TrianglesCompiler
{
public:
   TrianglesCompilerVertices(IActiveVertexMap& activeVertexMap,
      const Terrain::Model::DataBlock& dataBlock,
      TexturedVertexBuffer& vertexBuffer,
      const Vector3d& vOffset);
   virtual ~TrianglesCompilerVertices() throw() {}

   void Compile();

   const std::vector<unsigned int>& Indices() const throw() { return m_vecIndices; }

private:
   /// adds a vertex to the vertex list; must implement this in derived classes
   virtual void CompileVertex(unsigned int x, unsigned int y) override;

   void CalcNormal(unsigned int x, unsigned int y, Vector3d& vNormal);

private:
   const Terrain::Model::DataBlock& m_dataBlock;

   TexturedVertexBuffer& m_vertexBuffer;

   Vector3d m_vOffset;

   std::vector<unsigned int> m_vecIndices;
};

} // namespace Reduce
} // namespace Terrain
