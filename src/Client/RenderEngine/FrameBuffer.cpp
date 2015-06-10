//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file FrameBuffer.cpp OpenGL frame buffer class
//

// includes
#include "stdafx.h"
#include "FrameBuffer.hpp"
#include <boost/thread/once.hpp>

using OpenGL::FrameBuffer;

/// flag to only call Init() once
static boost::once_flag g_initFlag = BOOST_ONCE_INIT;

PFNGLGENBUFFERSARBPROC FrameBuffer::glGenFramebuffersARB = nullptr;
PFNGLBINDFRAMEBUFFERPROC FrameBuffer::glBindFramebufferARB = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC FrameBuffer::glDeleteFramebuffersARB = nullptr;
PFNGLFRAMEBUFFERTEXTUREARBPROC FrameBuffer::glFrameBufferTextureARB = nullptr;
PFNGLDRAWBUFFERSARBPROC FrameBuffer::glDrawBuffersARB = nullptr;
PFNGLFRAMEBUFFERRENDERBUFFERPROC FrameBuffer::glFramebufferRenderbufferARB = nullptr;

FrameBuffer::FrameBuffer() throw()
:m_uiFrameBufferName(0)
{
   // buffer object must be constructed in render context
   ATLASSERT(true == OpenGL::IsRenderThread());

   boost::call_once(&FrameBuffer::Init, g_initFlag);
}

void FrameBuffer::Init() throw()
{
   glGenFramebuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenFramebuffers");
   glBindFramebufferARB = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
   glDeleteFramebuffersARB = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
   glFrameBufferTextureARB = (PFNGLFRAMEBUFFERTEXTUREARBPROC)wglGetProcAddress("glFramebufferTextureARB");
   glDrawBuffersARB = (PFNGLDRAWBUFFERSARBPROC)wglGetProcAddress("glDrawBuffersARB");
   glFramebufferRenderbufferARB = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
}
