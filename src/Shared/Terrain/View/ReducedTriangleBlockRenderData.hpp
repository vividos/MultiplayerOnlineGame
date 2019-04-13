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
#include "Vector3.hpp"
#include "AABox.hpp"
#include "Texture.hpp"
#include "TexturedVertexBuffer.hpp"
#include "RangedAppendOnlyIndexBuffer.hpp"

// forward references
class GraphicsTaskManager;

namespace Terrain
{
class BlockTextureGenerator;

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

/// \brief block render data using the "reduced triangle" algorithm
/// \details using reduced triangles algorithm from:
/// http://people.llnl.gov/lindstrom2
/// http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.14.4771&rep=rep1&type=pdf
class TERRAIN_DECLSPEC ReducedTriangleBlockRenderData : public IBlockRenderData
{
public:
   /// ctor
   ReducedTriangleBlockRenderData(GraphicsTaskManager& taskManager,
      BlockTextureGenerator& blockTextureGenerator);
   virtual ~ReducedTriangleBlockRenderData() {}

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

   /// generates texture for block and uploads it
   void GenerateTexture(
      std::shared_ptr<Terrain::Model::DataBlock> spDataBlock);

   /// maps camera distance to level
   static unsigned int MapDistToLevel(double dist);

   /// fills zone level data
   void FillZoneLevelData(ZoneLevelData& data, std::shared_ptr<Terrain::Model::DataBlock> spZoneBlock,
      Terrain::Reduce::IActiveVertexMap& zoneActiveVertexMap, unsigned int uiLevel,
      unsigned int uiZoneOffsetX, unsigned int uiZoneOffsetY);

   /// uploads vertex and index buffers
   void UploadBuffers();

   /// renders single zone
   void RenderZone(const ZoneLevelData& data, unsigned int uiZoneX, unsigned int uiZoneY,
      const RenderOptions& renderOptions, const ViewFrustum3d& viewFrustum);

private:
   /// zone data, per level
   struct ZoneLevelData
   {
      ZoneLevelData()
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

   /// block texture generator
   BlockTextureGenerator& m_blockTextureGenerator;

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
   TexturePtr m_spTexture;

   /// infos for all zones and levels
   std::vector<ZoneLevelData> m_vecZoneLevelData;

   /// indicates if render data has been uploaded
   bool m_bUploaded;
};

} // namespace View
} // namespace Terrain
