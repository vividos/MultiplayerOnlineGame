//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ZoneManager.cpp Zone manager
//

// includes
#include "StdAfx.h"
#include "ZoneManager.hpp"
#include "Model/DataBlock.hpp"
#include "ReduceAlgorithm.hpp"
#include "TrianglesCompilerIndex.hpp"
#include "OpenGL.hpp"
#include "ViewFrustum3d.hpp"
#include "RenderOptions.hpp"
#include "GraphicsTaskManager.hpp"
#include <functional>

using Terrain::Reduce::ZoneManager;

/// number of levels of detail
const unsigned int c_uiNumLevels = 3;

/// number of zone divisions in each direction
const unsigned int c_uiNumZones = 8;

/// converts zone index to zone coords
static void IndexToZone(size_t uiIndex, unsigned int& xzone, unsigned int& yzone)
{
   xzone = uiIndex / c_uiNumZones;
   yzone = uiIndex % c_uiNumZones;
}

ZoneManager::ZoneManager(GraphicsTaskManager& taskManager,
                         const ArrayMapper2D& blockMapper,
                         RangedAppendOnlyIndexBuffer& indexBuffer, unsigned int uiBlockSize)
:m_taskManager(taskManager),
 m_blockMapper(blockMapper),
 m_indexBuffer(indexBuffer),
 m_uiZoneSize(uiBlockSize / c_uiNumZones)
{
   // per level data
   for (size_t i=0; i<c_uiNumLevels; i++)
      m_vecLevelData.push_back(LevelData(uiBlockSize));

   // per render data
   m_vecRenderData.resize(c_uiNumZones * c_uiNumZones);
}

unsigned int ZoneManager::NumZones() const throw()
{
   return c_uiNumZones;
}

bool ZoneManager::IsCurrentLevel(unsigned int uiZoneIndex, unsigned int uiLevel) const throw()
{
   const RenderData& renderData = m_vecRenderData[uiZoneIndex];
   // TODO remove const_cast: CurrentLevel() can be const?
   return const_cast<RenderData&>(renderData).CurrentLevel() == uiLevel;
}

/// \note non-blocking call!
void ZoneManager::PrepareZone(unsigned int xzone, unsigned int yzone,
                              unsigned int uiZoneIndex, unsigned int uiLevel,
                              std::shared_ptr<Terrain::Model::DataBlock> spZoneDataBlock)
{
   RenderData& renderData = m_vecRenderData[uiZoneIndex];

   if (renderData.CurrentLevel() == uiLevel)
      return;

   if (renderData.IsLevelPrepared(uiLevel))
   {
      renderData.CurrentLevel(uiLevel);
      return;
   }

   renderData.SetPreparedFlag(uiLevel);

   ATLTRACE(_T("Queue preparing zone %u, %u, level %u\n"), xzone, yzone, uiLevel);

   m_taskManager.BackgroundTaskGroup().Add(
      std::bind(&ZoneManager::PrepareRenderData, this, xzone, yzone, uiLevel,
         spZoneDataBlock, std::ref(renderData))
   );
}

/// \note executed in graphics worker thread
void ZoneManager::PrepareRenderData(unsigned int xzone, unsigned int yzone, unsigned int uiLevel,
   std::shared_ptr<Terrain::Model::DataBlock> spDataBlock,
   ZoneManager::RenderData& renderData)
{
   ATLTRACE(_T("Prepare zone %u, %u, level %u\n"), xzone, yzone, uiLevel);

   ActiveVertexMapWithEdgeInfo activeVertexMap =
      m_vecLevelData[uiLevel].m_activeVertexBlockMap.GetBlockWithEdgeInfo(xzone, yzone);

   unsigned int uiZoneSize = spDataBlock->GetElevationData().Size();

   std::vector<unsigned int> vecIndices;
   renderData.Prepare(uiLevel,
      xzone * uiZoneSize, yzone * uiZoneSize,
      spDataBlock, activeVertexMap, m_blockMapper, vecIndices);

   m_taskManager.UploadTaskGroup().Add(
      std::bind(&ZoneManager::UploadRenderData, this, uiLevel,
         std::ref(renderData),
         vecIndices));
}

void ZoneManager::UploadRenderData(unsigned int uiLevel, ZoneManager::RenderData& renderData, const std::vector<unsigned int>& vecIndices)
{
   ATLASSERT(OpenGL::IsRenderThread());

   IndexRange range = m_indexBuffer.Add(vecIndices);

   renderData.SetIndexRange(uiLevel, range);
}

void ZoneManager::Render(const RenderOptions& renderOptions, const ViewFrustum3d& viewFrustum)
{
   bool bDrawBoundingBox = renderOptions.Get(RenderOptions::optionTerrainZoneBoundingBox);
   bool bUseFrustumCulling = renderOptions.Get(RenderOptions::optionTerrainFrustumCulling);

   m_indexBuffer.Bind();

   // call all blocks to render themselves
   for (size_t i=0, iMax=m_vecRenderData.size(); i<iMax; i++)
   {
      if (bUseFrustumCulling)
      {
         unsigned int xzone, yzone;
         IndexToZone(i, xzone, yzone);

         ATLASSERT(xzone < c_uiNumZones);
         ATLASSERT(yzone < c_uiNumZones);

         const AABox& box1 = m_vecRenderData[i].GetBoundingBox();
         AABox box2;
         box2.UpdateBound(Vector3d(xzone * m_uiZoneSize, 0.0, yzone * m_uiZoneSize) + box1.Min());
         box2.UpdateBound(Vector3d(xzone * m_uiZoneSize, 0.0, yzone * m_uiZoneSize) + box1.Max());

         // check if given quad is visible
         if (ViewFrustum3d::resultOutside == viewFrustum.IsBoxInside(box2))
            continue;
      }

      m_vecRenderData[i].Render(m_indexBuffer, bDrawBoundingBox);
   }

   m_indexBuffer.Unbind();
}

//
// LevelData
//

ZoneManager::LevelData::LevelData(unsigned int uiBlockSize)
:m_activeVertexBlockMap(c_uiNumZones, uiBlockSize / c_uiNumZones)
{
   unsigned int uiZoneSize = uiBlockSize / c_uiNumZones;

   for (unsigned int xzone = 0; xzone < c_uiNumZones; xzone++)
   for (unsigned int yzone = 0; yzone < c_uiNumZones; yzone++)
   {
      // create an ActiveVertexMap, and initially fill it
      std::shared_ptr<ActiveVertexMap> spBlock(new ActiveVertexMap(uiZoneSize));
      spBlock->Init();

      m_activeVertexBlockMap.UpdateBlock(xzone, yzone, spBlock);
   }
}

//
// ZoneManager::RenderData
//

ZoneManager::RenderData::RenderData()
:m_uiCurrentLevel(c_uiNumLevels) // illegal value, so that level is loaded at first call to Prepare()
{
   m_vecLevelRanges.resize(c_uiNumLevels);
   m_vecIsPrepared.resize(c_uiNumLevels, 0);
}

bool ZoneManager::RenderData::IsLevelPrepared(unsigned int uiLevel) const throw()
{
   return m_vecIsPrepared[uiLevel] != 0;
}

void ZoneManager::RenderData::CurrentLevel(unsigned int uiLevel) throw()
{
   ATLASSERT(IsLevelPrepared(uiLevel) == true);

   if (m_vecLevelRanges[uiLevel] != NULL)
      m_uiCurrentLevel = uiLevel;
}

void ZoneManager::RenderData::SetPreparedFlag(unsigned int uiLevel) throw()
{
   m_vecIsPrepared[uiLevel] = 1;
}

void ZoneManager::RenderData::Prepare(unsigned int uiLevel,
   unsigned int xoffset, unsigned int yoffset,
   std::shared_ptr<Terrain::Model::DataBlock> spDataBlock,
   Terrain::Reduce::IActiveVertexMap& activeVertexMap,
   const ArrayMapper2D& blockMapper,
   std::vector<unsigned int>& vecIndices)
{
   if (m_vecLevelRanges[uiLevel] != NULL)
      return; // nothing to do

   // calculate bounding box
   {
      float fMin = std::numeric_limits<float>::infinity(),
         fMax = -std::numeric_limits<float>::infinity();

      const std::vector<float>& vecElevationData = spDataBlock->GetElevationData().RawData();
      for (size_t i=0, iMax=vecElevationData.size(); i<iMax; i++)
      {
         fMin = std::min(vecElevationData[i], fMin);
         fMax = std::max(vecElevationData[i], fMax);
      }

      m_boundingBox.UpdateBound(
         Vector3d(
            xoffset,
            fMin,
            yoffset));

      m_boundingBox.UpdateBound(
         Vector3d(
            xoffset + spDataBlock->GetElevationData().Size(),
            fMax,
            yoffset + spDataBlock->GetElevationData().Size()));
   }

   ReduceAlgorithmLevelOfDetail reducer(*spDataBlock);
   reducer.Reduce(activeVertexMap, uiLevel);

   TrianglesCompilerIndex compiler(activeVertexMap, blockMapper, *spDataBlock,
      xoffset, yoffset);
   compiler.Reduce();

   vecIndices = compiler.GetIndices();
}

void ZoneManager::RenderData::SetIndexRange(unsigned int uiLevel, const IndexRange& range)
{
   ATLASSERT(m_vecLevelRanges[uiLevel] == NULL);

   std::shared_ptr<IndexRange> spRange(new IndexRange(range));

   m_vecLevelRanges[uiLevel].swap(spRange);

   m_uiCurrentLevel = uiLevel;
}

void ZoneManager::RenderData::Render(RangedAppendOnlyIndexBuffer& indexBuffer, bool bDrawBoundingBox)
{
   ATLASSERT(OpenGL::IsRenderThread());

   if (m_uiCurrentLevel >= c_uiNumLevels ||
       m_vecLevelRanges[m_uiCurrentLevel] == NULL)
      return; // not prepared yet

#ifdef _DEBUG
   Color levelColors[3] =
   {
      Color(255, 255, 255),
      Color(0, 255, 0),
      Color(255, 0, 255)
   };

   glColor3ubv(levelColors[m_uiCurrentLevel].m_color);
#endif

   const IndexRange& range = *m_vecLevelRanges[m_uiCurrentLevel];

   indexBuffer.RenderRange(range);

   OpenGL::CountPolygons(range.m_uiSize / 3);

   // render bounding box
   if (bDrawBoundingBox)
   {
      Vector3d vMin = m_boundingBox.Min();
      Vector3d vMax = m_boundingBox.Max();

      glColor3ub(255,0,0);

      OpenGL::RenderBoundingBox(vMin, vMax);
   }
}
