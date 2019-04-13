//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file VertexBufferObject.hpp OpenGL VBO class
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include "OpenGL.hpp"

namespace OpenGL
{

/// \brief base class for vertex and index buffer objects
/// \details provides function pointers for ARB functions for VBO and IBO access
/// \note must be constructed when an OpenGL render context is already available
/// \see http://developer.nvidia.com/object/using_VBOs.html
/// \see http://www.songho.ca/opengl/gl_vbo.html
/// \see http://www.codeworx.org/opengl_tut45.php
/// \see http://www.opengl.org/wiki/Vertex_Specification_Best_Practices
class RENDERENGINE_DECLSPEC BufferObjectBase
{
public:
   /// ctor
   BufferObjectBase();

   /// returns if VBO extension is supported by OpenGL
   static bool IsAvailable()
   {
      return OpenGL::IsExtensionSupported(OpenGL::Extension::ARB_vertex_buffer_object);
   }

private:
   /// inits function pointers
   static void Init();

protected:
   // static data for access to vertex buffer object extension

   static PFNGLGENBUFFERSARBPROC glGenBuffersARB;  ///< generates buffer
   static PFNGLBINDBUFFERARBPROC glBindBufferARB;  ///< binds buffer
   static PFNGLBUFFERDATAARBPROC glBufferDataARB;  ///< sets data
   static PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB; ///< sets data range
   static PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;  ///< deletes buffer
   static PFNGLMAPBUFFERARBPROC glMapBufferARB; ///< maps buffer
   static PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB; ///< unmaps buffer
};


/// \brief vertex buffer object
/// \note must be constructed when an OpenGL render context is already available
template <typename TElement, GLenum target = GL_ARRAY_BUFFER_ARB, GLenum usage = GL_STATIC_DRAW_ARB>
class VertexBufferObject: public BufferObjectBase
{
   static_assert(
      target == GL_ARRAY_BUFFER_ARB ||
      target == GL_ELEMENT_ARRAY_BUFFER_ARB, "not an allowed value for target");

   static_assert(
      usage == GL_STATIC_DRAW_ARB ||
      usage == GL_STATIC_READ_ARB ||
      usage == GL_STATIC_COPY_ARB ||
      usage == GL_DYNAMIC_DRAW_ARB ||
      usage == GL_DYNAMIC_READ_ARB ||
      usage == GL_DYNAMIC_COPY_ARB ||
      usage == GL_STREAM_DRAW_ARB ||
      usage == GL_STREAM_READ_ARB ||
      usage == GL_STREAM_COPY_ARB, "not an allowed value for usage");

public:
   /// ctor
   VertexBufferObject()
      :m_uiVboName(0)
   {
   }
   /// dtor
   ~VertexBufferObject()
   {
      Clear();
   }

   /// generates VBO
   void Generate()
   {
      ATLASSERT(m_uiVboName == 0);
      ATLASSERT(glGenBuffersARB != NULL);

      glGenBuffersARB(1, &m_uiVboName);
      glTraceError("glGenBuffersARB");
   }

   /// binds to this VBO
   void Bind() const
   {
      ATLASSERT(glBindBufferARB != NULL);

      glBindBufferARB(target, m_uiVboName);
      glTraceError("glBindBufferARB");
   }

   /// unbinds VBO
   void Unbind() const
   {
      ATLASSERT(glBindBufferARB != NULL);

      glBindBufferARB(target, 0);
      glTraceError("glBindBufferARB");
   }

   /// frees VBO
   void Clear()
   {
      if (m_uiVboName == 0)
         return;

      ATLASSERT(glDeleteBuffersARB != NULL);

      glDeleteBuffersARB(1, &m_uiVboName);
      glTraceError("glDeleteBuffersARB");

      m_uiVboName = 0;
   }

   /// uploads data to VBO
   void Upload(const TElement* pData, unsigned int uiSizeInElements)
   {
      ATLASSERT(glBufferDataARB != NULL);

      glBufferDataARB(target, uiSizeInElements * sizeof(TElement), pData, usage);

      glTraceError("glBufferDataARB");
   }

   /// uploads data range to VBO
   void UploadRange(const TElement* pData, unsigned int uiOffset, unsigned int uiSizeInElements)
   {
      ATLASSERT(glBufferSubDataARB != NULL);

      glBufferSubDataARB(target, uiOffset * sizeof(TElement), uiSizeInElements * sizeof(TElement), pData, usage);

      glTraceError("glBufferSubDataARB");
   }

   /// maps VBO into client memory
   void* MapBuffer(GLenum access)
   {
      return glMapBufferARB(target, access);
   }

   /// unmaps a buffer
   void UnmapBuffer()
   {
      ATLASSERT(glUnmapBufferARB != NULL);

      glUnmapBufferARB(target);
   }

private:
   /// VBO name
   GLuint m_uiVboName;
};

/// float VBO type
typedef VertexBufferObject<GLfloat, GL_ARRAY_BUFFER_ARB, GL_STATIC_DRAW_ARB> VertexBufferObjectFloat;

/// unsigned int VBO type
typedef VertexBufferObject<unsigned int, GL_ELEMENT_ARRAY_BUFFER_ARB, GL_STATIC_DRAW_ARB> IndexBufferObject32;

/// unsigned short VBO type
typedef VertexBufferObject<unsigned short, GL_ELEMENT_ARRAY_BUFFER_ARB, GL_STATIC_DRAW_ARB> IndexBufferObject16;

} // namespace OpenGL
