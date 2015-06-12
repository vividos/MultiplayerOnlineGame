//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ReducedTriangleBlockRenderData.cpp Block render data for the "reduced triangle" algorithm
//

// includes
#include "StdAfx.h"
#include "ReducedTriangleBlockRenderData.hpp"
#include "Model/DataBlock.hpp"
#include "Reduce/ZoneDataSource.hpp"
#include "Reduce/ActiveVertexBlockMap.hpp"
#include "Reduce/ReduceAlgorithm.hpp"
#include "Reduce/TrianglesCompilerVertices.hpp"
#include "GraphicsTaskManager.hpp"
#include "BlockTextureGenerator.hpp"
#include "ArrayMapper2D.hpp"
#include "RenderOptions.hpp"
#include "ViewFrustum3d.hpp"
#include "TraceOutputStopwatch.hpp"
#include "HighResolutionTimer.hpp"
#include "TextureParameter.hpp"

using Terrain::View::ReducedTriangleBlockRenderData;

/// number of levels of detail
const unsigned int c_uiNumLevels = 3;

/// number of zone divisions in each direction
const unsigned int c_uiNumZones = 8;

ReducedTriangleBlockRenderData::ReducedTriangleBlockRenderData(
   GraphicsTaskManager& taskManager,
   Terrain::BlockTextureGenerator& blockTextureGenerator)
:m_taskManager(taskManager),
 m_blockTextureGenerator(blockTextureGenerator),
 m_uiZoneSize(0),
 m_vertexBuffer(0),
 m_bUploaded(false)
{
}

void ReducedTriangleBlockRenderData::Prepare(std::shared_ptr<Terrain::Model::DataBlock> spDataBlock)
{
   TraceOutputStopwatch<HighResolutionTimer> stopwatch(_T("ReducedTriangleBlockRenderData::Prepare"));

   // prepare all levels of all zones in block
   m_vecZoneLevelData.resize(c_uiNumZones * c_uiNumZones * c_uiNumLevels);

   m_uiZoneSize = spDataBlock->GetElevationData().Size() / c_uiNumZones;

   ArrayMapper2D zoneMapper(c_uiNumZones, c_uiNumZones);

   Terrain::Reduce::ZoneDataSource dataSource(*spDataBlock, m_uiZoneSize);

   for (unsigned int uiLevel=0; uiLevel<c_uiNumLevels; uiLevel++)
   {
      size_t uiBaseIndex = c_uiNumZones * c_uiNumZones * uiLevel;

      /// active vertex map for this level
      Reduce::ActiveVertexBlockMap levelActiveVertexBlockMap(
         c_uiNumZones, m_uiZoneSize);

      {
         for (unsigned int uiZoneX = 0; uiZoneX<c_uiNumZones; uiZoneX++)
         for (unsigned int uiZoneY = 0; uiZoneY<c_uiNumZones; uiZoneY++)
         {
            // create an ActiveVertexMap, and initially fill it
            std::shared_ptr<Reduce::ActiveVertexMap> spBlock(new Reduce::ActiveVertexMap(m_uiZoneSize));
            spBlock->Init();

            levelActiveVertexBlockMap.UpdateBlock(uiZoneX, uiZoneY, spBlock);
         }
      }

      for (unsigned int uiZoneY = 0; uiZoneY<c_uiNumZones; uiZoneY++)
      for (unsigned int uiZoneX = 0; uiZoneX<c_uiNumZones; uiZoneX++)
      {
         std::shared_ptr<Terrain::Model::DataBlock> spZoneBlock =
            dataSource.LoadBlock(uiZoneX * m_uiZoneSize, uiZoneY * m_uiZoneSize, m_uiZoneSize);

         ZoneLevelData data;

         Reduce::ActiveVertexMapWithEdgeInfo zoneActiveVertexMap =
            levelActiveVertexBlockMap.GetBlockWithEdgeInfo(uiZoneX, uiZoneY);

         FillZoneLevelData(data, spZoneBlock,
            zoneActiveVertexMap, uiLevel,
            uiZoneX * m_uiZoneSize, uiZoneY * m_uiZoneSize);

         size_t uiZoneIndex = zoneMapper.CoordToIndex(uiZoneX, uiZoneY);

         m_vecZoneLevelData[uiBaseIndex + uiZoneIndex] = data;
      }
   }

   m_taskManager.UploadTaskGroup().Add(
      std::bind(&ReducedTriangleBlockRenderData::GenerateTexture, this, spDataBlock));
}

void ReducedTriangleBlockRenderData::Upload()
{
   m_indexBuffer.Update();
   m_vertexBuffer.Upload();

   m_bUploaded = true;
}

void ReducedTriangleBlockRenderData::GenerateTexture(
   std::shared_ptr<Terrain::Model::DataBlock> spDataBlock)
{
   ATLASSERT(m_bUploaded == false); // must occur before Upload() has been called

   m_spTexture = m_blockTextureGenerator.Generate(spDataBlock);
}

void CalcMinMax(const std::vector<float>& vecElevationData, float& fMin, float& fMax)
{
   fMin = std::numeric_limits<float>::infinity();
   fMax = -std::numeric_limits<float>::infinity();

   for (size_t i=0, iMax=vecElevationData.size(); i<iMax; i++)
   {
      fMin = std::min(vecElevationData[i], fMin);
      fMax = std::max(vecElevationData[i], fMax);
   }
}

unsigned int ReducedTriangleBlockRenderData::MapDistToLevel(double dist) throw()
{
   unsigned int uiLevel = 0;
   if (dist > 128.0 && dist < 256.0)
      uiLevel = 1;
   else
   if (dist >= 256.0)
      uiLevel = 2;

   return uiLevel;
}

void ReducedTriangleBlockRenderData::FillZoneLevelData(ZoneLevelData& data,
   std::shared_ptr<Terrain::Model::DataBlock> spZoneBlock,
   Terrain::Reduce::IActiveVertexMap& zoneActiveVertexMap, unsigned int uiLevel,
   unsigned int uiZoneOffsetX, unsigned int uiZoneOffsetY)
{
   data.m_bValid = true;

   // bounding box
   {
      float fMin, fMax;
      const std::vector<float>& vecElevationData = spZoneBlock->GetElevationData().RawData();
      CalcMinMax(vecElevationData, fMin, fMax);

      data.m_boundingBox.UpdateBound(
         Vector3d(
            uiZoneOffsetX,
            fMin,
            uiZoneOffsetY));

      data.m_boundingBox.UpdateBound(
         Vector3d(
            uiZoneOffsetX + spZoneBlock->GetElevationData().Size(),
            fMax,
            uiZoneOffsetY + spZoneBlock->GetElevationData().Size()));
   }

   // generate data, store in vertex and index buffer
   {
      std::vector<unsigned int> vecIndices;

      Reduce::ReduceAlgorithmLevelOfDetail reducer(*spZoneBlock);
      reducer.Reduce(zoneActiveVertexMap, uiLevel);

      Vector3d vZoneOffset(uiZoneOffsetX, 0.0, uiZoneOffsetY);

      Reduce::TrianglesCompilerVertices compiler(zoneActiveVertexMap, *spZoneBlock,
         m_vertexBuffer, vZoneOffset);
      compiler.Compile();

      vecIndices = compiler.Indices();

      data.m_indexRange = m_indexBuffer.Add(vecIndices);
   }
}

void ReducedTriangleBlockRenderData::Update(const Vector3d& vCamera)
{
   m_vPosition = vCamera;
}

void ReducedTriangleBlockRenderData::Render(const RenderOptions& renderOptions, const ViewFrustum3d& viewFrustum)
{
   if (!m_bUploaded)
      return;

   bool bFilled = renderOptions.Get(RenderOptions::optionTerrainFilled);
   glPolygonMode(GL_FRONT, bFilled ? GL_FILL : GL_LINE);

   m_vertexBuffer.Bind();
   m_indexBuffer.Bind();

   if (m_spTexture != nullptr)
      m_spTexture->Bind();

   TextureParameter<GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, GL_MODULATE>::Use();

   ArrayMapper2D zoneMapper(c_uiNumZones, c_uiNumZones);

   // do all zones
   for (unsigned int uiZoneY = 0; uiZoneY<c_uiNumZones; uiZoneY++)
   for (unsigned int uiZoneX = 0; uiZoneX<c_uiNumZones; uiZoneX++)
   {
      // figure out level
      double xmid = (uiZoneX * m_uiZoneSize) + m_uiZoneSize/2.0;
      double ymid = (uiZoneY * m_uiZoneSize) + m_uiZoneSize/2.0;

      double dist = (m_vPosition - Vector3d(xmid, m_vPosition.Y(), ymid)).Length();

      unsigned int uiLevel = MapDistToLevel(dist);


      size_t uiBaseIndex = c_uiNumZones * c_uiNumZones * uiLevel;

      const ZoneLevelData& data = m_vecZoneLevelData[
         uiBaseIndex + zoneMapper.CoordToIndex(uiZoneX, uiZoneY)];

#ifdef _DEBUG
      // in debug, color zones by level
      Color levelColors[3] =
      {
         Color(255, 255, 255),
         Color(0, 255, 0),
         Color(255, 0, 255)
      };

      ATLASSERT(uiLevel < _countof(levelColors));
      glColor3ubv(levelColors[uiLevel].m_color);
#endif

      RenderZone(data, uiZoneX, uiZoneY, renderOptions, viewFrustum);
   }

   m_indexBuffer.Unbind();
   m_vertexBuffer.Unbind();
}

void ReducedTriangleBlockRenderData::RenderZone(const ZoneLevelData& data,
   unsigned int uiZoneX, unsigned int uiZoneY,
   const RenderOptions& renderOptions, const ViewFrustum3d& viewFrustum)
{
   bool bDrawBoundingBox = renderOptions.Get(RenderOptions::optionTerrainZoneBoundingBox);
   bool bUseFrustumCulling = renderOptions.Get(RenderOptions::optionTerrainFrustumCulling);

   if (bUseFrustumCulling)
   {
      AABox box;
      Vector3d vOffset(uiZoneX * m_uiZoneSize, 0.0, uiZoneY * m_uiZoneSize);

      box.UpdateBound(vOffset + data.m_boundingBox.Min());
      box.UpdateBound(vOffset + data.m_boundingBox.Max());

      // check if given quad is visible
      if (ViewFrustum3d::resultOutside == viewFrustum.IsBoxInside(box))
         return; // not visible
   }

   m_indexBuffer.RenderRange(data.m_indexRange);
   OpenGL::CountPolygons(data.m_indexRange.m_uiSize / 3);

   if (bDrawBoundingBox)
   {
      Vector3d vMin = data.m_boundingBox.Min();
      Vector3d vMax = data.m_boundingBox.Max();

      glColor3ub(255,255,255);

      OpenGL::RenderBoundingBox(vMin, vMax);
   }
}
