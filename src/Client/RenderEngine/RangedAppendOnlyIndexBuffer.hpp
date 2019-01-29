//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file RangedAppendOnlyIndexBuffer.hpp Index buffer for range of indices
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include "VertexBufferObject.hpp"

/// range to render
struct IndexRange
{
   /// ctor
   IndexRange()
      :m_uiStart(0),
       m_uiSize(0),
       m_uiFirstIndex(0),
       m_uiLastIndex(0)
   {
   }

   unsigned int m_uiStart;       ///< start offset in buffer
   unsigned int m_uiSize;        ///< number of elements
   unsigned int m_uiFirstIndex;  ///< lowest index used in this range
   unsigned int m_uiLastIndex;   ///< highest index used in this range
};

/// index buffer object that manages ranges of indices, and that can be appended to only
class RENDERENGINE_DECLSPEC RangedAppendOnlyIndexBuffer: public boost::noncopyable
{
public:
   /// ctor
   RangedAppendOnlyIndexBuffer();

   /// adds new indices to index buffer object; may be called from worker thread
   IndexRange Add(const std::vector<unsigned int>& vecData);

   /// updates index buffer; must be called in render thread
   void Update();

   /// binds index buffer for usage
   void Bind() const
   {
      ATLASSERT(m_spIndexBuffer != NULL);

      glEnableClientState(GL_INDEX_ARRAY);
      m_spIndexBuffer->Bind();
   }

   /// renders given range
   void RenderRange(const IndexRange& range) const;

   /// unbinds index buffer
   void Unbind() const
   {
      ATLASSERT(m_spIndexBuffer != NULL);

      m_spIndexBuffer->Unbind();
      glDisableClientState(GL_INDEX_ARRAY);
   }

private:
   /// index buffer object type
   typedef OpenGL::VertexBufferObject<unsigned int,
      GL_ELEMENT_ARRAY_BUFFER_ARB,
      GL_DYNAMIC_DRAW_ARB> T_IndexBuffer;

   /// index buffer
   std::unique_ptr<T_IndexBuffer> m_spIndexBuffer;

   /// cached indices
   std::vector<unsigned int> m_vecIndices;

   /// GL function to draw range of elements
   PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
};
