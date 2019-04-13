//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Terrain/View/TerrainRenderManager.cpp Terrain render manager
//

// includes
#include "StdAfx.h"
#include "TerrainRenderManager.hpp"
#include "RenderEngine.hpp"
#include "GraphicsTaskManager.hpp"
#include "IBlockRenderData.hpp"
#include "BlockRenderDataVertexBuffer.hpp"
#include "BlockRenderDataTexturedVertexBuffer.hpp"
#include "ReducedTriangleBlockRenderData.hpp"
#include "Model/DataBlockManager.hpp"
#include <functional>
#include "Vector2.hpp"
#include "ViewFrustum3d.hpp"
#include "PerspectiveCamera.hpp"
#include "OpenGL.hpp"

/// time interval after we re-check for blocks we must load or we can delete
double c_dBlockUpdateCycleInSec = 1.0;

/// radius of circle where blocks should be visible
const double c_dBlockViewRadius = 200.0;

using namespace Terrain::View;

TerrainRenderManager::TerrainRenderManager(RenderEngine& engine, GraphicsTaskManager& taskManager,
                                 Terrain::Model::DataBlockManager& dataBlockManager)
:m_engine(engine),
 m_taskManager(taskManager),
 m_dataBlockManager(dataBlockManager),
 m_bCleanup(false)
{
}

void TerrainRenderManager::SetPosition(const Vector3d& vPosition, double /*dAngleDir*/)
{
   m_vPosition = vPosition;
}

void TerrainRenderManager::Prepare()
{
//   ATLTRACE(_T("TerrainRenderManager::Prepare()\n"));

   // start preparing blocks that are in the position's neighborhood

   // check all blocks within radius
   const unsigned int uiBlockSize = Terrain::Model::DataBlockManager::BlockSize();
   unsigned int uiCenterX = unsigned(m_vPosition.X()) / uiBlockSize;
   unsigned int uiCenterY = unsigned(m_vPosition.Z()) / uiBlockSize;
   {
      Vector2d vCircleCenter(m_vPosition.X(), m_vPosition.Z());

      for (unsigned int uiOffsetX = uiCenterX == 0 ? 1 : 0; uiOffsetX<3; uiOffsetX++)
      for (unsigned int uiOffsetY = uiCenterY == 0 ? 1 : 0; uiOffsetY<3; uiOffsetY++)
      {
         unsigned int uiBlockX = uiCenterX + uiOffsetX - 1;
         unsigned int uiBlockY = uiCenterY + uiOffsetY - 1;

         if (m_dataBlockManager.IsAvail(uiBlockX, uiBlockY))
            continue;

         Vector2d vBlockBase(uiBlockX * uiBlockSize, uiBlockY * uiBlockSize);

         if (!IntersectSquareCircle(vBlockBase, 512.0, vCircleCenter, c_dBlockViewRadius))
            continue;

         m_dataBlockManager.AsyncLoadBlock(uiBlockX, uiBlockY,
            std::bind(&TerrainRenderManager::OnDataBlockLoaded, this, uiBlockX, uiBlockY, std::placeholders::_1));
      }
   }

   if (!m_bCleanup)
      m_blockUpdateTimer.Start();
}

void TerrainRenderManager::Render(RenderOptions& options)
{
   CheckPrepareTimer();

   // get view frustum
   std::shared_ptr<PerspectiveCamera> spCamera =
      std::dynamic_pointer_cast<PerspectiveCamera>(m_engine.GetCamera());

   ViewFrustum3d viewFrustum = spCamera->GetViewFrustum();

   // render all blocks within radius
   const unsigned int uiBlockSize = Terrain::Model::DataBlockManager::BlockSize();
   unsigned int uiCenterX = unsigned(m_vPosition.X()) / uiBlockSize;
   unsigned int uiCenterY = unsigned(m_vPosition.Z()) / uiBlockSize;
   {
      Vector2d vCircleCenter(m_vPosition.X(), m_vPosition.Z());

      for (unsigned int uiOffsetX = uiCenterX == 0 ? 1 : 0; uiOffsetX<3; uiOffsetX++)
      for (unsigned int uiOffsetY = uiCenterY == 0 ? 1 : 0; uiOffsetY<3; uiOffsetY++)
      {
         unsigned int uiBlockX = uiCenterX + uiOffsetX - 1;
         unsigned int uiBlockY = uiCenterY + uiOffsetY - 1;

         Vector2d vBlockBase(uiBlockX * uiBlockSize, uiBlockY * uiBlockSize);

         if (IntersectSquareCircle(vBlockBase, 512.0, vCircleCenter, c_dBlockViewRadius))
         {
            // calc block relative position
            Vector3d vBlockPosition = m_vPosition - Vector3d(vBlockBase.X(), 0.0, vBlockBase.Y());

            RenderBlock(options, viewFrustum, uiBlockX, uiBlockY, vBlockBase, vBlockPosition);
         }
      }
   }
}

void TerrainRenderManager::Done()
{
   m_bCleanup = true;
   m_blockUpdateTimer.Stop();
}

void TerrainRenderManager::CheckPrepareTimer()
{
   if (m_bCleanup)
      return;

   if (m_blockUpdateTimer.Elapsed() > c_dBlockUpdateCycleInSec)
   {
      m_blockUpdateTimer.Stop();
      m_blockUpdateTimer.Reset();

      m_taskManager.BackgroundTaskGroup().Add(
         std::bind(&TerrainRenderManager::Prepare, this));
   }
}

void TerrainRenderManager::RenderBlock(RenderOptions& renderOptions, ViewFrustum3d& viewFrustum,
                                  unsigned int uiBlockX, unsigned int uiBlockY,
                                  const Vector2d& vBlockBase, const Vector3d& vBlockPosition)
{
   if (m_renderDataMap.IsAvail(uiBlockX, uiBlockY))
   {
      std::shared_ptr<IBlockRenderData> spRenderData = m_renderDataMap.Get(uiBlockX, uiBlockY);
      ATLASSERT(spRenderData != nullptr); // must have been prepared

      spRenderData->Update(vBlockPosition);

      glPushMatrix();
      glTranslated(vBlockBase.X(), 0.0, vBlockBase.Y());

      spRenderData->Render(renderOptions, viewFrustum);

      glPopMatrix();
   }
   else
   {
      // TODO draw white rectangle as placeholder?
   }
}

void TerrainRenderManager::OnDataBlockLoaded(unsigned int xblock, unsigned int yblock,
                                        std::shared_ptr<Terrain::Model::DataBlock> spDataBlock)
{
   m_taskManager.BackgroundTaskGroup().Add(
      std::bind(&TerrainRenderManager::PrepareRenderData, this, xblock, yblock, spDataBlock));
}

/// \note executed in background thread
void TerrainRenderManager::PrepareRenderData(unsigned int xblock, unsigned int yblock,
                                        std::shared_ptr<Terrain::Model::DataBlock> spDataBlock)
{
   std::shared_ptr<IBlockRenderData> spRenderData(
      new ReducedTriangleBlockRenderData(m_taskManager, m_blockTextureGenerator));
//      new BlockRenderDataTexturedVertexBuffer(m_taskManager));
//      new BlockRenderDataVertexBuffer);

   spRenderData->Prepare(spDataBlock);

   m_renderDataMap.Store(xblock, yblock, spRenderData);

   m_taskManager.UploadTaskGroup().Add(
      std::bind(&IBlockRenderData::Upload, spRenderData));
}
