//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TrianglesCompilerVertices.cpp triangles compiler using vertex buffer object
//

// includes
#include "StdAfx.h"
#include "TrianglesCompilerVertices.hpp"
#include "TexturedVertexBuffer.hpp"
#include "Model/DataBlock.hpp"

using Terrain::Reduce::TrianglesCompilerVertices;
using Terrain::Reduce::IActiveVertexMap;

TrianglesCompilerVertices::TrianglesCompilerVertices(IActiveVertexMap& activeVertexMap,
   const Terrain::Model::DataBlock& dataBlock,
   TexturedVertexBuffer& vertexBuffer,
   const Vector3d& vOffset)
:TrianglesCompiler(activeVertexMap),
 m_dataBlock(dataBlock),
 m_vertexBuffer(vertexBuffer),
 m_vOffset(vOffset)
{
   // reserve size for indices, expecting about 12.5% triangles to be rendered
   unsigned int uiMaxTriangleNum = dataBlock.GetElevationData().RawData().size() * 2;
   m_vecIndices.reserve(uiMaxTriangleNum / 8);
}

void TrianglesCompilerVertices::Compile()
{
   TrianglesCompiler::Compile(m_dataBlock.GetElevationData().Size());
}

void TrianglesCompilerVertices::CompileVertex(unsigned int x, unsigned int y)
{
   unsigned int uiVertexIndex = m_vertexBuffer.Vertices().size();
   m_vecIndices.push_back(uiVertexIndex);

   VertexBufferEntry entry;

   Vector3d vVertex(x, m_dataBlock.GetElevationData().Height(x, y), y);
   vVertex += m_vOffset;

   entry.vertex[0] = static_cast<GLfloat>(vVertex.X());
   entry.vertex[1] = static_cast<GLfloat>(vVertex.Y());
   entry.vertex[2] = static_cast<GLfloat>(vVertex.Z());

   Vector3d vNormal(0.0, 1.0, 0.0);
   CalcNormal(x, y, vNormal);

   entry.normal[0] = static_cast<GLfloat>(vNormal.X());
   entry.normal[1] = static_cast<GLfloat>(vNormal.Y());
   entry.normal[2] = static_cast<GLfloat>(vNormal.Z());

   unsigned int uiBlockSize = m_dataBlock.GetElevationData().Size();
   entry.texcoords[0] = float(x) / uiBlockSize;
   entry.texcoords[1] = float(y) / uiBlockSize;

   entry.color[0] = entry.color[1] = entry.color[2] = 255;
   entry.color[3] = 0;

   m_vertexBuffer.Vertices().push_back(entry);
}

void TrianglesCompilerVertices::CalcNormal(unsigned int /*x*/, unsigned int /*y*/, Vector3d& /*vNormal*/)
{
   // TODO impl
}
