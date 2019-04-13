//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file BlockRenderDataVertexBuffer.hpp Render data for rendering terrain using vertex buffer
//
#pragma once

// includes
#include "TerrainCommon.hpp"
#include "IBlockRenderData.hpp"
#include "VertexBufferObject.hpp"

#undef USE_USHORT

namespace Terrain
{
namespace View
{

/// \brief render data for rendering terrain using VBO
/// \details just throws vertices and indices in two VBOs; no LOD optimization
class TERRAIN_DECLSPEC BlockRenderDataVertexBuffer: public IBlockRenderData
{
public:
   /// ctor
   BlockRenderDataVertexBuffer()
      :m_uiVertexCount(0),
       m_uiIndexCount(0)
   {
   }

   /// dtor
   virtual ~BlockRenderDataVertexBuffer() {}

   virtual void Prepare(std::shared_ptr<Model::DataBlock> spDataBlock) override;
   virtual void Upload() override;
   virtual void Update(const Vector3d& vCamera) override;
   virtual void Render(const RenderOptions& renderOptions, const ViewFrustum3d& viewFrustum) override;

private:
   std::vector<float> m_vecVertexCoords;

#ifdef USE_USHORT
   std::vector<unsigned short> m_vecIndex;
#else
   std::vector<unsigned int> m_vecIndex;
#endif

   /// vertex buffer
   OpenGL::VertexBufferObjectFloat m_vertData;

   /// index buffer
#ifdef USE_USHORT
   OpenGL::IndexBufferObject16 m_indexData;
#else
   OpenGL::IndexBufferObject32 m_indexData;
#endif

   /// vertex count
   size_t m_uiVertexCount;

   /// index count
   size_t m_uiIndexCount;
};

} // namespace View
} // namespace Terrain
