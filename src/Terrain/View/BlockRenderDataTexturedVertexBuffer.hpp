//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file BlockRenderDataTexturedVertexBuffer.hpp Render data for rendering terrain using vertex buffer
//
#pragma once

// includes
#include "TerrainCommon.hpp"
#include "IBlockRenderData.hpp"
#include "Texture.hpp"
#include "TexturedVertexBuffer.hpp"
#include "RangedAppendOnlyIndexBuffer.hpp"
#include "Vector3.hpp"
#include "SafeFlag.hpp"

// forward references
class GraphicsTaskManager;
class RenderOptions;

namespace Terrain
{
namespace Reduce
{
class ZoneManager;
}

/// \brief render data for terrain block, consisting of several terrain zones, and using VBO
/// \details using reduced triangles algorithm from:
/// http://people.llnl.gov/lindstrom2
/// http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.14.4771&rep=rep1&type=pdf
class TERRAIN_DECLSPEC BlockRenderDataTexturedVertexBuffer: public View::IBlockRenderData
{
public:
   /// ctor
   BlockRenderDataTexturedVertexBuffer(GraphicsTaskManager& taskManager) throw();
   /// dtor
   virtual ~BlockRenderDataTexturedVertexBuffer() throw() {}

   /// prepares render data; called only once
   virtual void Prepare(std::shared_ptr<Model::DataBlock> spDataBlock) override;

   /// updates render data; called every frame
   virtual void Update(const Vector3d& vCamera) override;

   /// renders terrain block; called every frame
   virtual void Render(const RenderOptions& renderOptions,
      const ViewFrustum3d& viewFrustum) override;

private:
   /// prepares vertex array for whole block
   void PrepareVertexArray(Model::DataBlock& dataBlock);

   /// uploads vertex array; must be called in render thread
   void UploadVertexArray();

   /// generates texture bitmap for block
   void FillTextureBitmap(Bitmap& bmp);

   /// update blocks, given new camera position
   void UpdateBlocks(const Vector3d& vCamera);

   /// maps camera distance to zone level
   static unsigned int MapDistToLevel(double dist) throw();

private:
   /// task manager
   GraphicsTaskManager& m_taskManager;

   /// data block
   std::shared_ptr<Terrain::Model::DataBlock> m_spDataBlock;

   /// zone manager
   std::shared_ptr<Terrain::Reduce::ZoneManager> m_spZoneManager;

   /// vertex buffer
   TexturedVertexBuffer m_vertData;

   /// flag that indicates if vertex buffer has been uploaded
   SafeFlag m_vertexBufferUploaded;

   /// index buffer
   RangedAppendOnlyIndexBuffer m_indexBuffer;

   /// block texture
   Texture m_tex;

   /// indicates if first update was already done
   bool m_bFirstUpdate;

   /// camera position at last update
   Vector3d m_vLastCamera;

   /// indicates if block update is in progress
   SafeFlag m_blockUpdateInProgress;
};

} // namespace Terrain
