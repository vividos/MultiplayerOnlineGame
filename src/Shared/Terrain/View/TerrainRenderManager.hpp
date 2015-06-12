//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file src/Terrain/View/TerrainRenderManager.hpp Terrain render manager
//
#pragma once

// includes
#include "TerrainCommon.hpp"
#include "IRenderable.hpp"
#include "BlockRenderDataMap.hpp"
#include "BlockTextureGenerator.hpp"
#include "Timer.hpp"
#include "Vector3.hpp"
#include <atomic>

// forward references
class Vector2d;
class ViewFrustum3d;
class RenderEngine;
class GraphicsTaskManager;

namespace Terrain
{
namespace Model
{
class DataBlock;
class DataBlockManager;
}

namespace View
{

/// terrain render manager
class TERRAIN_DECLSPEC TerrainRenderManager: public IRenderable
{
public:
   /// ctor
   TerrainRenderManager(RenderEngine& engine, GraphicsTaskManager& taskManager,
      Terrain::Model::DataBlockManager& dataBlockManager);
   /// dtor
   virtual ~TerrainRenderManager() throw() {}

   /// sets new LOD center position
   void SetPosition(const Vector3d& vPosition, double dAngleDir) throw();

   // virtual methods from IRenderable
   virtual bool IsPrepareNeeded() const override { return true; }
   virtual bool IsUploadNeeded() const override { return false; }
   virtual void Prepare() override;
   virtual void Upload() override {}
   virtual void Render(RenderOptions& options) override;
   virtual void Done() override;

private:
   void OnDataBlockLoaded(unsigned int xblock, unsigned int yblock,
      std::shared_ptr<Model::DataBlock> spDataBlock);

   void PrepareRenderData(unsigned int xblock, unsigned int yblock,
      std::shared_ptr<Terrain::Model::DataBlock> spDataBlock);

   void CheckPrepareTimer();

   void RenderBlock(RenderOptions& renderOptions, ViewFrustum3d& viewFrustum,
      unsigned int uiBlockX, unsigned int uiBlockY,
      const Vector2d& vBlockBase, const Vector3d& vBlockPosition);

private:
   /// render engine
   RenderEngine& m_engine;

   /// task manager
   GraphicsTaskManager& m_taskManager;

   /// texture generator for blocks
   Terrain::BlockTextureGenerator m_blockTextureGenerator;

   /// data block manager
   Terrain::Model::DataBlockManager& m_dataBlockManager;

   /// render data map
   BlockRenderDataMap m_renderDataMap;

   /// current camera position
   Vector3d m_vPosition;

   /// timer to update blocks
   Timer m_blockUpdateTimer;

   /// flag to indicate cleanup phase
   std::atomic<bool> m_bCleanup;
};

} // namespace View
} // namespace Terrain
