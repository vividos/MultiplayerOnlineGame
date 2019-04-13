//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file FrameBuffer.hpp OpenGL frame buffer class
//
#pragma once

// includes
#include "Texture.hpp"

namespace OpenGL
{
class RenderBuffer;

/// \brief framebuffer object
/// \note must be constructed when an OpenGL render context is already available
class RENDERENGINE_DECLSPEC FrameBuffer
{
public:
   /// ctor
   FrameBuffer();
   /// dtor
   ~FrameBuffer()
   {
      Clear();
   }

   /// returns if frame buffer extension is supported by OpenGL
   static bool IsAvailable()
   {
      return
         OpenGL::IsExtensionSupported(OpenGL::Extension::ARB_framebuffer_object) &&
         OpenGL::IsExtensionSupported(OpenGL::Extension::ARB_draw_buffers);
   }

   /// generates framebuffer
   void Generate()
   {
      ATLASSERT(m_uiFrameBufferName == 0);
      ATLASSERT(glGenFramebuffersARB != NULL);

      glGenFramebuffersARB(1, &m_uiFrameBufferName);
      glTraceError("glGenFramebuffersARB");
   }

   /// binds to this frame buffer
   void Bind() const
   {
      ATLASSERT(m_uiFrameBufferName != 0);
      ATLASSERT(glBindFramebufferARB != NULL);

      glBindFramebufferARB(GL_FRAMEBUFFER, m_uiFrameBufferName);
      glTraceError("glBindFramebufferARB");
   }

   /// unbinds frame buffer
   void Unbind() const
   {
      ATLASSERT(glBindFramebufferARB != NULL);

      glBindFramebufferARB(GL_FRAMEBUFFER, 0);
      glTraceError("glBindFramebufferARB");
   }

   /// frees framebuffer
   void Clear()
   {
      if (m_uiFrameBufferName == 0)
         return;

      ATLASSERT(glDeleteFramebuffersARB != NULL);

      glDeleteFramebuffersARB(1, &m_uiFrameBufferName);
      glTraceError("glDeleteFramebuffersARB");

      m_uiFrameBufferName = 0;
   }

   /// sets texture as target for frame buffer rendering
   void SetTexture(TexturePtr spTexture)
   {
      ATLASSERT(m_uiFrameBufferName != 0);
      ATLASSERT(glFrameBufferTextureARB != NULL);
      ATLASSERT(glDrawBuffersARB != NULL);

      glFrameBufferTextureARB(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, spTexture->Id(), 0);
      glTraceError("glFrameBufferTextureARB");

      GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
      glDrawBuffersARB(1, DrawBuffers);
      glTraceError("glDrawBuffersARB");
   }

   /// sets render buffer as component of this framebuffer
   void SetRenderBuffer(RenderBuffer& /*renderBuffer*/, GLuint uiComponent)
   {
      ATLASSERT(m_uiFrameBufferName != 0);
      ATLASSERT(glFramebufferRenderbufferARB != NULL);

      ATLASSERT(uiComponent == GL_DEPTH_ATTACHMENT);

      uiComponent;
      //glFramebufferRenderbufferARB(GL_FRAMEBUFFER, uiComponent, m_uiFrameBufferName, renderBuffer.Id());
      //glTraceError("glFramebufferRenderbufferARB");
   }

private:
   /// inits function pointers
   static void Init();

private:
   /// framebuffer name
   GLuint m_uiFrameBufferName;

   // static data for access to framebuffer object extension

   static PFNGLGENFRAMEBUFFERSPROC glGenFramebuffersARB;  ///< generates framebuffer
   static PFNGLBINDFRAMEBUFFERPROC glBindFramebufferARB;  ///< binds framebuffer
   static PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffersARB;  ///< deletes framebuffer
   static PFNGLFRAMEBUFFERTEXTUREARBPROC glFrameBufferTextureARB; ///< sets framebuffer to texture
   static PFNGLDRAWBUFFERSARBPROC glDrawBuffersARB; ///< sets draw buffers
   static PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbufferARB; ///< sets render buffer
};

} // namespace OpenGL
