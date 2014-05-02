//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TrianglesCompilerIndex.cpp triangles compiler using an index buffer
//

// includes
#include "StdAfx.h"
#include "TrianglesCompilerIndex.hpp"
#include "ActiveVertexMap.hpp"
#include "Model/DataBlock.hpp"

using Terrain::Reduce::TrianglesCompilerIndex;

void TrianglesCompilerIndex::Reduce()
{
   const Model::ElevationData& elevationData = m_dataBlock.GetElevationData();

   // reserve size for indices, expecting about 12.5% triangles to be rendered
   unsigned int uiMaxTriangleNum = elevationData.RawData().size() * 2;
   m_vecIndex.reserve(uiMaxTriangleNum * 1 / 8);

   // compile
   Compile(elevationData.Size());
}

void TrianglesCompilerIndex::CompileVertex(unsigned int x, unsigned int y)
{
#ifdef _DEBUG
   const Model::ElevationData& elevationData = m_dataBlock.GetElevationData();

   ATLASSERT(x <= elevationData.Size());
   ATLASSERT(y <= elevationData.Size());
#endif

   unsigned int uiOffset = m_blockMapper.CoordToIndex(m_xoffset+x, m_yoffset+y);
   m_vecIndex.push_back(uiOffset);
}
