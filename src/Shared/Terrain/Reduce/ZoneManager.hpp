//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ZoneManager.hpp Zone manager
//
#pragma once

// includes
#include "ActiveVertexBlockMap.hpp"
#include "AABox.hpp"
#include "ArrayMapper2D.hpp"
#include "RangedAppendOnlyIndexBuffer.hpp"

// forward references
class ViewFrustum3d;
class GraphicsTaskManager;
class RenderOptions;

namespace Terrain
{
namespace Model
{
class DataBlock;
}
namespace Reduce
{
class ITrianglesCompiler;

/// \brief zone manager
/// \details manages terrain data block divided up into zones
class ZoneManager: public boost::noncopyable
{
public:
   /// ctor
   ZoneManager(GraphicsTaskManager& taskManager,
      const ArrayMapper2D& blockMapper,
      RangedAppendOnlyIndexBuffer& indexBuffer,
      unsigned int uiBlockSize);

   /// returns number of zones in one direction (square zone matrix)
   unsigned int NumZones() const;

   /// returns if given level is current level, for given zone index
   bool IsCurrentLevel(unsigned int uiZoneIndex, unsigned int uiLevel) const;

   /// prepares render data for given zone; non-blocking call
   void PrepareZone(unsigned int xzone, unsigned int yzone,
      unsigned int uiZoneIndex, unsigned int uiLevel, std::shared_ptr<Terrain::Model::DataBlock> spZoneDataBlock);

   /// renders all blocks
   void Render(const RenderOptions& renderOptions, const ViewFrustum3d& viewFrustum);

private:
   class RenderData;

   /// prepares render data for given zone; called in worker thread
   void PrepareRenderData(unsigned int xzone, unsigned int yzone, unsigned int uiLevel,
      std::shared_ptr<Terrain::Model::DataBlock> spZoneDataBlock,
      RenderData& renderData);

   /// uploads given render data; called in render thread
   void UploadRenderData(unsigned int uiLevel, ZoneManager::RenderData& renderData,
      const std::vector<unsigned int>& vecIndices);

private:
   /// task manager
   GraphicsTaskManager& m_taskManager;

   /// mapper for vertex indices of whole block
   ArrayMapper2D m_blockMapper;

   /// index buffer for indices of all zones
   RangedAppendOnlyIndexBuffer& m_indexBuffer;

   /// zone size
   unsigned int m_uiZoneSize;

   /// per-level data
   struct LevelData
   {
      /// ctor
      LevelData(unsigned int uiZoneSize);

      /// active vertex map for all sub blocks
      ActiveVertexBlockMap m_activeVertexBlockMap;
   };

   /// data for all levels
   std::vector<LevelData> m_vecLevelData;

   /// per-zone render data
   class RenderData
   {
   public:
      /// ctor
      RenderData();

      // get methods

      /// returns current level of detail
      unsigned int CurrentLevel() const { return m_uiCurrentLevel; }

      /// returns if given level is already prepared for rendering
      bool IsLevelPrepared(unsigned int uiLevel) const;

      /// returns bounding box
      const AABox& GetBoundingBox() const { return m_boundingBox; }

      // actions

      /// sets current level
      void CurrentLevel(unsigned int uiLevel);

      /// sets prepared flag
      void SetPreparedFlag(unsigned int uiLevel);

      /// starts preparing new level
      void Prepare(unsigned int uiNewLevel,
         unsigned int xoffset, unsigned int yoffset,
         std::shared_ptr<Terrain::Model::DataBlock> spDataBlock,
         IActiveVertexMap& activeVertexMap,
         const ArrayMapper2D& blockMapper,
         std::vector<unsigned int>& vecIndices);

      /// sets index range for this zone
      void SetIndexRange(unsigned int uiLevel, const IndexRange& range);

      /// renders zone using triangle index range
      void Render(RangedAppendOnlyIndexBuffer& indexBuffer, bool bDrawBoundingBox = false);

   private:
      /// number of current level
      unsigned int m_uiCurrentLevel;

      /// flag to indicate that a level is prepared
      std::vector<unsigned int> m_vecIsPrepared;

      /// index buffer ranges for all levels in this zone
      std::vector<std::shared_ptr<IndexRange>> m_vecLevelRanges;

      /// bounding box of zone
      AABox m_boundingBox;
   };

   /// render data for all zones
   std::vector<RenderData> m_vecRenderData;
};

} // namespace Reduce
} // namespace Terrain
