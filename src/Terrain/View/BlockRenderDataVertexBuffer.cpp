//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file BlockRenderDataVertexBuffer.cpp Render data for rendering terrain using vertex buffer
//

// includes
#include "StdAfx.h"
#include "BlockRenderDataVertexBuffer.hpp"
#include "Model/DataBlock.hpp"
#include "OpenGL.hpp"
#include <vector>

using Terrain::Model::DataBlock;
using Terrain::View::BlockRenderDataVertexBuffer;

void BlockRenderDataVertexBuffer::Prepare(std::shared_ptr<DataBlock> spDataBlock)
{
   DataBlock& dataBlock = *spDataBlock;

   ATLASSERT(OpenGL::BufferObjectBase::IsAvailable() == true);

   const std::vector<float>& vecElevationData = dataBlock.GetElevationData().RawData();

   // generate all vertices
   std::vector<Vector3d> vecVertices(vecElevationData.size());

   for (size_t i=0, iMax=vecVertices.size(); i<iMax; i++)
      vecVertices[i] = dataBlock.GetElevationData().VertexByIndex(i);

   // convert to float array
   m_vecVertexCoords.resize(vecVertices.size()*3);
   for (size_t i=0, iMax=vecVertices.size(); i<iMax; i++)
   {
      m_vecVertexCoords[i*3+0] = static_cast<float>(vecVertices[i].X());
      m_vecVertexCoords[i*3+1] = static_cast<float>(vecVertices[i].Y());
      m_vecVertexCoords[i*3+2] = static_cast<float>(vecVertices[i].Z());
   }

   m_uiVertexCount = vecVertices.size();


   // generate index array
   m_vecIndex.reserve(vecElevationData.size()*6);

   const ArrayMapper2D& mapper = spDataBlock->GetElevationData().ArrayMapper();

   const unsigned int uiMaxCoord = spDataBlock->GetElevationData().Size();

   unsigned int uiStep = 4;
   for (unsigned int x=0; x<uiMaxCoord; x += uiStep)
   for (unsigned int y=0; y<uiMaxCoord; y += uiStep)
   {
#ifdef USE_USHORT
      // first triangle
      m_vecIndex.push_back(static_cast<unsigned short>(mapper.CoordToIndex(x+0,      y+0)));
      m_vecIndex.push_back(static_cast<unsigned short>(mapper.CoordToIndex(x+0,      y+uiStep)));
      m_vecIndex.push_back(static_cast<unsigned short>(mapper.CoordToIndex(x+uiStep, y+0)));

      // second triangle
      m_vecIndex.push_back(static_cast<unsigned short>(mapper.CoordToIndex(x+0,      y+uiStep)));
      m_vecIndex.push_back(static_cast<unsigned short>(mapper.CoordToIndex(x+uiStep, y+uiStep)));
      m_vecIndex.push_back(static_cast<unsigned short>(mapper.CoordToIndex(x+uiStep, y+0)));
#else
      // first triangle
      m_vecIndex.push_back(mapper.CoordToIndex(x+0,      y+0));
      m_vecIndex.push_back(mapper.CoordToIndex(x+0,      y+uiStep));
      m_vecIndex.push_back(mapper.CoordToIndex(x+uiStep, y+0));
                         
      // second triangle 
      m_vecIndex.push_back(mapper.CoordToIndex(x+0,      y+uiStep));
      m_vecIndex.push_back(mapper.CoordToIndex(x+uiStep, y+uiStep));
      m_vecIndex.push_back(mapper.CoordToIndex(x+uiStep, y+0));

#endif
   }

   m_uiIndexCount = m_vecIndex.size();
}

void BlockRenderDataVertexBuffer::Upload()
{
   // upload vertex array
   m_vertData.Generate();
   m_vertData.Bind();
   m_vertData.Upload(&m_vecVertexCoords[0], m_vecVertexCoords.size());

   m_vecVertexCoords.clear();

   // upload index array
   m_indexData.Generate();
   m_indexData.Bind();
   m_indexData.Upload(&m_vecIndex[0], m_vecIndex.size());

   m_vecIndex.clear();
}

void BlockRenderDataVertexBuffer::Update(const Vector3d& /*vDist*/)
{
   // do nothing
}

void BlockRenderDataVertexBuffer::Render(const RenderOptions& /*renderOptions*/,
                                         const ViewFrustum3d& /*viewFrustum*/)
{
   glPolygonMode(GL_FRONT, GL_LINE);

   m_vertData.Bind();

   glEnableClientState(GL_VERTEX_ARRAY);

   glVertexPointer(3, GL_FLOAT, 0, (GLvoid*)NULL);

   m_indexData.Bind();

   // draw triangles with m_uiIndexCount index (must be dividable by 3)
#ifdef USE_USHORT
   glDrawElements(GL_TRIANGLES, m_uiIndexCount, GL_UNSIGNED_SHORT, NULL);
#else
   glDrawElements(GL_TRIANGLES, m_uiIndexCount, GL_UNSIGNED_INT, NULL);
#endif

   glDisableClientState(GL_VERTEX_ARRAY);

   OpenGL::CountPolygons(m_uiIndexCount/3);
}
