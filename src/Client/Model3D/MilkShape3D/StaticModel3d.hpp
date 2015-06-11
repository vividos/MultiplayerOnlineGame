//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkShape3D\StaticModel3d.hpp MilkShape3D static model
//
#pragma once

// includes
#include "Model3dBase.hpp"
#include "TexturedVertexBuffer.hpp"
#include "RangedAppendOnlyIndexBuffer.hpp"
#include "MilkShape3D/Data.hpp"
#include "Matrix4.hpp"
#include "AABox.hpp"
#include <atomic>

// forward references
class RenderOptions;

namespace MilkShape3D
{

/// \brief static Milkshape 3D model
/// \details A model without movable joints. Since the whole data for a static
/// model doesn't change, vertex and index buffer are stored directly in this
/// class.
class StaticModel3d: public Model3dBase
{
public:
   /// ctor
   StaticModel3d() throw()
      :m_vbo(0)
   {
   }
   /// dtor
   virtual ~StaticModel3d() throw() {}

   /// prepares model for rendering
   virtual void Prepare() throw() override;

   /// uploads data to graphics card
   virtual void Upload() throw() override;

   /// returns the texture name of the (hopefully) only one material of the model
   CString SingleMaterialTextureName() const;

   /// renders static model
   void Render(RenderOptions& options);

private:
   /// prepares vertex buffer
   void PrepareVertices();

   /// prepares index buffer
   void PrepareIndices();

private:
   /// bounding box
   AABox m_boundingBox;

   /// vertex buffer
   TexturedVertexBuffer m_vbo;

   /// indes buffer
   RangedAppendOnlyIndexBuffer m_ibo;

   /// range to draw
   IndexRange m_indexRange;

   std::atomic<bool> m_bAlreadyUploaded;
};

} // namespace MilkShape3D
