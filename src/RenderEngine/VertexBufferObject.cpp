//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file VertexBufferObject.cpp OpenGL VBO class
//

// includes
#include "StdAfx.h"
#include "VertexBufferObject.hpp"
#include "OpenGL.hpp"
#include <boost/thread/once.hpp>

using OpenGL::BufferObjectBase;

/// flag to only call Init() once
static boost::once_flag g_initFlag = BOOST_ONCE_INIT;

PFNGLGENBUFFERSARBPROC BufferObjectBase::glGenBuffersARB = 0;
PFNGLBINDBUFFERARBPROC BufferObjectBase::glBindBufferARB = 0;
PFNGLBUFFERDATAARBPROC BufferObjectBase::glBufferDataARB = 0;
PFNGLBUFFERSUBDATAARBPROC BufferObjectBase::glBufferSubDataARB = 0;
PFNGLDELETEBUFFERSARBPROC BufferObjectBase::glDeleteBuffersARB = 0;
PFNGLMAPBUFFERARBPROC BufferObjectBase::glMapBufferARB = 0;
PFNGLUNMAPBUFFERARBPROC BufferObjectBase::glUnmapBufferARB = 0;

BufferObjectBase::BufferObjectBase() throw()
{
   // buffer object must be constructed in render context
   ATLASSERT(true == OpenGL::IsRenderThread());

   boost::call_once(&BufferObjectBase::Init, g_initFlag);
}

void BufferObjectBase::Init() throw()
{
   glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
   glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
   glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
   glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
   glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
   glMapBufferARB = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
   glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");
}
