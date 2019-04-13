//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file StaticIndexBuffer.hpp Static index buffer
//
#pragma once

// includes
#include "VertexBufferObject.hpp"

/// static draw index buffer
class StaticIndexBuffer: public boost::noncopyable
{
public:
   /// ctor
   StaticIndexBuffer()
      :m_uiIndexCount(0)
   {
   }

   /// returns index buffer entries array
         std::vector<unsigned int>& IndexBuffer() { return m_vecIndices; }
   /// returns index buffer entries array; const version
   const std::vector<unsigned int>& IndexBuffer() const { return m_vecIndices; }

   /// uploads index buffer
   void Upload(bool bClearStorage = true)
   {
      ATLASSERT(OpenGL::IsRenderThread());
      ATLASSERT(OpenGL::BufferObjectBase::IsAvailable() == true);

      // always reset: replace the previous buffer
      m_spVbo.reset(new T_IndexBuffer);
      m_spVbo->Generate();

      m_spVbo->Bind();

      m_spVbo->Upload(&m_vecIndices[0], m_vecIndices.size());

      m_uiIndexCount = m_vecIndices.size();

      // clear storage
      if (bClearStorage)
      {
         std::vector<unsigned int>().swap(m_vecIndices);
      }
   }

   /// binds index buffer for usage
   void Bind() const
   {
      ATLASSERT(OpenGL::IsRenderThread());
      ATLASSERT(m_spVbo != NULL);

      glEnableClientState(GL_INDEX_ARRAY);
      m_spVbo->Bind();
   }

   /// renders from index buffer (vertex buffer must already be bound!)
   void Render() const
   {
      ATLASSERT(OpenGL::IsRenderThread());

      glDrawElements(GL_TRIANGLES, m_uiIndexCount, GL_UNSIGNED_INT, NULL);

      OpenGL::CountPolygons(m_uiIndexCount / 3);
   }

   /// unbinds index buffer
   void Unbind() const
   {
      ATLASSERT(OpenGL::IsRenderThread());
      ATLASSERT(m_spVbo != NULL);

      m_spVbo->Unbind();
      glDisableClientState(GL_INDEX_ARRAY);
   }

private:
   /// index buffer object type
   typedef OpenGL::VertexBufferObject<unsigned int,
      GL_ELEMENT_ARRAY_BUFFER_ARB,
      GL_STATIC_DRAW_ARB> T_IndexBuffer;

   /// index buffer
   std::unique_ptr<T_IndexBuffer> m_spVbo;

   /// cached indices
   std::vector<unsigned int> m_vecIndices;

   /// number of indices in index buffer
   size_t m_uiIndexCount;
};
