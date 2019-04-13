//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file RenderEngine.cpp Render engine class
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
   Impl()
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

RenderEngine::RenderEngine(const CString& cszWindowCaption, unsigned int uiWidth, unsigned int uiHeight, bool bFullscreen)
:m_spImpl(new Impl)
{
   m_spImpl->m_spWindow.reset(new RenderWindow(cszWindowCaption, uiWidth, uiHeight, bFullscreen));

   ApplyRenderOptions(GetRenderOptions());

   // call IsExtensionSupported here so that it gets initialized
   OpenGL::IsExtensionSupported(_T(""));

   // set our thread as render thread
   OpenGL::SetRenderThreadId();
}

RenderEngine::RenderEngine(std::shared_ptr<SDL_Window> spWindow)
:m_spImpl(new Impl)
{
   m_spImpl->m_spWindow.reset(new RenderWindow(spWindow));

   ApplyRenderOptions(GetRenderOptions());

   // call IsExtensionSupported here so that it gets initialized
   OpenGL::IsExtensionSupported(_T(""));

   // set our thread as render thread
   OpenGL::SetRenderThreadId();
}

RenderEngine::~RenderEngine()
{
   ATLASSERT(m_spImpl != nullptr);

   if (m_spImpl->m_spScenegraph != nullptr)
      m_spImpl->m_spScenegraph->Done();

   m_spImpl->m_spScenegraph.reset();
   m_spImpl.reset();
}

void RenderEngine::SetScenegraph(std::shared_ptr<IScenegraph> spScenegraph)
{
   ATLASSERT(m_spImpl != nullptr);

   m_spImpl->m_spScenegraph = spScenegraph;
}

void RenderEngine::SetCamera(std::shared_ptr<ICamera> spCamera)
{
   ATLASSERT(m_spImpl != nullptr);

   m_spImpl->m_spCamera = spCamera;
}

RenderOptions& RenderEngine::GetRenderOptions()
{
   return m_spImpl->m_renderOptions;
}

std::shared_ptr<ICamera> RenderEngine::GetCamera()
{
   ATLASSERT(m_spImpl != nullptr);

   return m_spImpl->m_spCamera;
}

RenderWindow& RenderEngine::GetRenderWindow()
{
   ATLASSERT(m_spImpl != nullptr);
   ATLASSERT(m_spImpl->m_spWindow != nullptr);

   return *m_spImpl->m_spWindow;
}

void RenderEngine::Render()
{
   ATLASSERT(OpenGL::IsRenderThread());

   glClearColor(0,0,0,0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   ApplyRenderOptions(GetRenderOptions());

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
   ATLASSERT(m_spImpl != nullptr);

   if (m_spImpl->m_spWindow != nullptr)
      m_spImpl->m_spWindow->SwapBuffers();
}

void RenderEngine::ApplyRenderOptions(RenderOptions& renderOptions)
{
   if (renderOptions.Get(RenderOptions::optionCullBackface))
   {
      // only render front face, counter clockwise
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
      glFrontFace(GL_CCW);
      glPolygonMode(GL_FRONT, GL_FILL);
   }
   else
   {
      // render back faces, too
      glDisable(GL_CULL_FACE);
      glCullFace(GL_BACK);
      glFrontFace(GL_CCW);
      glPolygonMode(GL_FRONT, GL_FILL);

      if (renderOptions.Get(RenderOptions::optionBackfaceAsLines))
         glPolygonMode(GL_BACK, GL_LINE);
      else
         glPolygonMode(GL_BACK, GL_FILL);
   }

   // enable z-buffer
   glEnable(GL_DEPTH_TEST);

   // enable texturing
   glEnable(GL_TEXTURE_2D);

   // smooth shading
   glShadeModel(GL_SMOOTH);

   // give some rendering hints
   glHint(GL_FOG_HINT, GL_DONT_CARE);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);

   // reset errors
   glGetError();
}
