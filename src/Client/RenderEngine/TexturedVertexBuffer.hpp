//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TexturedVertexBuffer.hpp Vertex buffer object for textured triangles
//
#pragma once

/// includes
#include "RenderEngineCommon.hpp"
#include "OpenGL.hpp"
#include "VertexBufferObject.hpp"
#include "Vector3.hpp"
#include "TexCoord2f.hpp"
#include "Color.hpp"
#include <vector>

/// VBO entry for vertex, normal, texture coordinates and color
__declspec(align(1))
struct VertexBufferEntry
{
   // default ctor
   VertexBufferEntry()
   {
   }

   /// ctor
   VertexBufferEntry(const Vector3d& vVertex, const Vector3d& vNormal, const TexCoord2f& texCoord, const Color& c)
   {
      vertex[0] = static_cast<GLfloat>(vVertex.X());
      vertex[1] = static_cast<GLfloat>(vVertex.Y());
      vertex[2] = static_cast<GLfloat>(vVertex.Z());

      normal[0] = static_cast<GLfloat>(vNormal.X());
      normal[1] = static_cast<GLfloat>(vNormal.Y());
      normal[2] = static_cast<GLfloat>(vNormal.Z());

      texcoords[0] = static_cast<GLfloat>(texCoord.U());
      texcoords[1] = static_cast<GLfloat>(texCoord.V());

      color[0] = c.m_color[0];
      color[1] = c.m_color[1];
      color[2] = c.m_color[2];
      color[3] = c.m_color[3];
   }

   GLfloat vertex[3];      ///< vertex coordinates
   GLfloat normal[3];      ///< normal coordinates
   GLfloat texcoords[2];   ///< texture coordinates
   GLubyte color[4];       ///< color values
};

static_assert(sizeof(VertexBufferEntry) == 36, "VertexBufferEntry must have exactly 36 bytes");


/// VBO that contains vertices of textured triangles
template <GLenum usage>
class TexturedVertexBufferT
{
public:
   /// ctor
   TexturedVertexBufferT(unsigned int uiSize = 0)
      :m_vecVertices(uiSize)
   {
   }

   /// returns vertex buffer entries array
         std::vector<VertexBufferEntry>& Vertices()       throw() { return m_vecVertices; }
   /// returns vertex buffer entries array; const version
   const std::vector<VertexBufferEntry>& Vertices() const throw() { return m_vecVertices; }

   /// uploads vertex buffer
   void Upload(bool bClearStorage = true)
   {
      ATLASSERT(OpenGL::IsRenderThread());
      ATLASSERT(OpenGL::BufferObjectBase::IsAvailable() == true);

      if (m_spVbo == NULL)
      {
         m_spVbo.reset(new T_VertexBuffer);
         m_spVbo->Generate();
      }

      m_spVbo->Bind();

#ifdef _DEBUG
      // must have alignment of 1
      unsigned int uiDist = reinterpret_cast<unsigned int>(&m_vecVertices[1])
         - reinterpret_cast<unsigned int>(&m_vecVertices[0]);
      ATLASSERT(uiDist == sizeof(VertexBufferEntry));
#endif

      m_spVbo->Upload(&m_vecVertices[0], m_vecVertices.size());

      // clear storage
      if (bClearStorage)
      {
         std::vector<VertexBufferEntry>().swap(m_vecVertices);
      }
   }

   /// binds vertex buffer for usage
   void Bind() const
   {
      ATLASSERT(OpenGL::IsRenderThread());

      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glEnableClientState(GL_COLOR_ARRAY);

      m_spVbo->Bind();

      const GLsizei stride = sizeof(VertexBufferEntry);

      glVertexPointer(3, GL_FLOAT, stride, NULL);
      glNormalPointer(GL_FLOAT, stride, (const GLvoid*)offsetof(VertexBufferEntry, normal));
      glTexCoordPointer(2, GL_FLOAT, stride, (const GLvoid*)offsetof(VertexBufferEntry, texcoords));
      glColorPointer(3, GL_UNSIGNED_BYTE, stride, (const GLvoid*)offsetof(VertexBufferEntry, color));
   }

   /// unbinds vertex buffer
   void Unbind() const
   {
      ATLASSERT(OpenGL::IsRenderThread());

      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_NORMAL_ARRAY);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      glDisableClientState(GL_COLOR_ARRAY);

      m_spVbo->Unbind();
   }

protected:
   /// VBO type
   typedef OpenGL::VertexBufferObject<VertexBufferEntry,
      GL_ARRAY_BUFFER_ARB,
      usage> T_VertexBuffer;

   /// vertex buffer
   std::unique_ptr<T_VertexBuffer> m_spVbo;

private:
   /// vertex entries
   std::vector<VertexBufferEntry> m_vecVertices;
};

/// textured vertex buffer, static draw
typedef TexturedVertexBufferT<GL_STATIC_DRAW_ARB> TexturedVertexBuffer;

/// textured vertex buffer, dynamic draw
class DynamicUpdateTexturedVertexBuffer: public TexturedVertexBufferT<GL_DYNAMIC_DRAW_ARB>
{
   /// base class type
   typedef TexturedVertexBufferT<GL_DYNAMIC_DRAW_ARB> BaseClass;

public:
   /// ctor
   DynamicUpdateTexturedVertexBuffer(unsigned int uiSize)
      :BaseClass(uiSize)
   {
   }

   /// uploads VBO
   void Upload()
   {
      BaseClass::Upload(true);
   }

   /// maps uploaded VBO into client memory
   VertexBufferEntry* MapBuffer()
   {
      void* pData = m_spVbo->MapBuffer(GL_WRITE_ONLY_ARB);
      return reinterpret_cast<VertexBufferEntry*>(pData);
   }

   /// unmaps buffer
   void UnmapBuffer()
   {
      m_spVbo->UnmapBuffer();
   }
};
