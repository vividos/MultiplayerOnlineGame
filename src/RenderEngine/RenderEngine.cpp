//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file RenderEngine.cpp Render engine class
//

// includes
#include "StdAfx.h"
#include "RenderEngine.hpp"
#include <SDL.h>
#include "OpenGL.hpp"
#include "RenderWindow.hpp"
#include "RenderOptions.hpp"
#include "IScenegraph.hpp"
#include "ICamera.hpp"

/// render engine impl class
class RenderEngine::Impl: public boost::noncopyable
{
public:
   /// ctor
   Impl() throw()
   {
   }

   /// render window
   std::shared_ptr<RenderWindow> m_spWindow;

   /// current render options
   RenderOptions m_renderOptions;

   /// current camera
   std::shared_ptr<ICamera> m_spCamera;

   /// current scenegraph
   std::shared_ptr<IScenegraph> m_spScenegraph;
};

RenderEngine::RenderEngine(unsigned int uiWidth, unsigned int uiHeight, bool bFullscreen)
:m_spImpl(new Impl)
{
   m_spImpl->m_spWindow.reset(new RenderWindow(uiWidth, uiHeight, bFullscreen, m_spImpl->m_renderOptions));

   m_spImpl->m_spWindow->SetupView(GetRenderOptions());

   // call IsExtensionSupported here so that it gets initialized
   OpenGL::IsExtensionSupported(_T(""));

   // set our thread as render thread
   OpenGL::SetRenderThreadId();
}

RenderEngine::~RenderEngine()
{
   ATLASSERT(m_spImpl != NULL);

   if (m_spImpl->m_spScenegraph != nullptr)
      m_spImpl->m_spScenegraph->Done();

   m_spImpl->m_spScenegraph.reset();
   m_spImpl.reset();
}

void RenderEngine::SetScenegraph(std::shared_ptr<IScenegraph> spScenegraph) throw()
{
   ATLASSERT(m_spImpl != NULL);

   m_spImpl->m_spScenegraph = spScenegraph;
}

void RenderEngine::SetCamera(std::shared_ptr<ICamera> spCamera)
{
   ATLASSERT(m_spImpl != NULL);

   m_spImpl->m_spCamera = spCamera;
}

RenderOptions& RenderEngine::GetRenderOptions()
{
   return m_spImpl->m_renderOptions;
}

std::shared_ptr<ICamera> RenderEngine::GetCamera()
{
   ATLASSERT(m_spImpl != NULL);

   return m_spImpl->m_spCamera;
}

void RenderEngine::Render()
{
   ATLASSERT(OpenGL::IsRenderThread());

   glClearColor(0,0,0,0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();

   // position camera
   if (m_spImpl->m_spCamera != nullptr)
      m_spImpl->m_spCamera->Use();

   // render scenegraph
   if (m_spImpl->m_spScenegraph != nullptr)
      m_spImpl->m_spScenegraph->Render(GetRenderOptions());
}

void RenderEngine::SwapBuffers()
{
   ATLASSERT(OpenGL::IsRenderThread());
   ATLASSERT(m_spImpl != NULL);

   if (m_spImpl->m_spWindow != nullptr)
      m_spImpl->m_spWindow->SwapBuffers();
}
