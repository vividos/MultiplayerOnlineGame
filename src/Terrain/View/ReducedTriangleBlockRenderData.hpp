//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ReducedTriangleBlockRenderData.hpp Block render data for the "reduced triangle" algorithm
//
#pragma once

// includes
#include "TerrainCommon.hpp"
#include "IBlockRenderData.hpp"
#include "Vector3d.hpp"
#include "AABox.hpp"
#include "Texture.hpp"
#include "TexturedVertexBuffer.hpp"
#include "RangedAppendOnlyIndexBuffer.hpp"

// forward references
class GraphicsTaskManager;

namespace Terrain
{
namespace Model
{
class DataBlock;
}
namespace Reduce
{
class IActiveVertexMap;
}
namespace View
{

/// block render data using the "reduced triangle" algorithm
class TERRAIN_DECLSPEC ReducedTriangleBlockRenderData: public IBlockRenderData
{
public:
   ReducedTriangleBlockRenderData(GraphicsTaskManager& taskManager) throw();
   virtual ~ReducedTriangleBlockRenderData() throw() {}

private:
   // virtual methods from IBlockRenderData

   /// prepares render data; called only once
   virtual void Prepare(std::shared_ptr<Terrain::Model::DataBlock> spDataBlock) override;

   /// updates render data; called only once
   virtual void Upload() override;

   /// updates render data; called every frame
   virtual void Update(const Vector3d& vCamera) override;

   /// renders terrain block; called every frame
   virtual void Render(const RenderOptions& renderOptions, const ViewFrustum3d& viewFrustum) override;

private:
   struct ZoneLevelData;

   static unsigned int MapDistToLevel(double dist) throw();

   void FillZoneLevelData(ZoneLevelData& data, std::shared_ptr<Terrain::Model::DataBlock> spZoneBlock,
      Terrain::Reduce::IActiveVertexMap& zoneActiveVertexMap, unsigned int uiLevel,
      unsigned int uiZoneOffsetX, unsigned int uiZoneOffsetY);

   void UploadBuffers();

   void RenderZone(const ZoneLevelData& data, unsigned int uiZoneX, unsigned int uiZoneY,
      const RenderOptions& renderOptions, const ViewFrustum3d& viewFrustum);

private:
   /// zone data, per level
   struct ZoneLevelData
   {
      ZoneLevelData() throw()
         :m_bValid(false)
      {
      }

      /// indicate if valid
      bool m_bValid;

      /// index range of vertices to render for this zone and level
      IndexRange m_indexRange;

      /// bounding box of zone
      AABox m_boundingBox;
   };

   /// task manager
   GraphicsTaskManager& m_taskManager;

   /// current camera position
   Vector3d m_vPosition;

   /// zone size
   unsigned int m_uiZoneSize;

   // render related data

   /// vertex buffer
   TexturedVertexBuffer m_vertexBuffer;

   /// index buffer
   RangedAppendOnlyIndexBuffer m_indexBuffer;

   /// block texture
   Texture m_texture;

   /// infos for all zones and levels
   std::vector<ZoneLevelData> m_vecZoneLevelData;

   /// indicates if render data has been uploaded
   bool m_bUploaded;
};

} // namespace View
} // namespace Terrain
