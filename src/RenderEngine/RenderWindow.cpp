//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file RenderWindow.cpp Render engine class
//

// includes
#include "StdAfx.h"
#include "RenderWindow.hpp"
#include <SDL.h>
#include "OpenGL.hpp"

RenderWindow::RenderWindow(const CString& cszWindowCaption, unsigned int uiWidth, unsigned int uiHeight, bool bFullscreen)
{
   InitVideo(cszWindowCaption, uiWidth, uiHeight, bFullscreen);

   // call IsExtensionSupported here so that it gets initialized
   OpenGL::IsExtensionSupported(_T(""));

   // set our thread as render thread
   OpenGL::SetRenderThreadId();
}

void RenderWindow::SwapBuffers()
{
   ATLASSERT(OpenGL::IsRenderThread());
   ATLASSERT(m_spWindow != nullptr);

   SDL_GL_SwapWindow(m_spWindow.get());
}

void RenderWindow::SetFullscreen(bool bFullscreen)
{
   ATLASSERT(m_spWindow != nullptr);

   SDL_SetWindowFullscreen(m_spWindow.get(), bFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void RenderWindow::SetCaption(const CString& cszCaption)
{
   ATLASSERT(m_spWindow != nullptr);

   CStringA cszaCaption(cszCaption);
   SDL_SetWindowTitle(m_spWindow.get(), cszaCaption);
}

void RenderWindow::SetMousePos(int xpos, int ypos)
{
   ATLASSERT(m_spWindow != nullptr);

   SDL_WarpMouseInWindow(m_spWindow.get(), static_cast<Uint16>(xpos), static_cast<Uint16>(ypos));
}

void RenderWindow::GetWindowSize(int& iWidth, int& iHeight) const
{
   ATLASSERT(m_spWindow != nullptr);

   SDL_GetWindowSize(m_spWindow.get(), &iWidth, &iHeight);
}

void RenderWindow::InitVideo(const CString& cszWindowCaption, unsigned int uiWidth, unsigned int uiHeight, bool bFullscreen)
{
   // output SDL version number
   {
      SDL_version linked = {0};
      SDL_GetVersion(&linked);

      CString cszText;
      cszText.Format(_T("using SDL %u.%u.%u"), linked.major, linked.minor, linked.patch);
      LOG_INFO(cszText, Log::Client::Renderer);
   }

   // first, initialize SDL's video subsystem
   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      CString cszText;
      cszText.Format(_T("video initialization failed: %hs"), SDL_GetError());

      LOG_ERROR(cszText, Log::Client::Renderer);
      throw Exception(cszText, __FILE__, __LINE__);
   }

   // setup video mode
   int iFlags = SDL_WINDOW_OPENGL | (bFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

   {
      CString cszText;
      cszText.Format(_T("setting video mode: %u x %u%s"),
         uiWidth, uiHeight,
         bFullscreen ? _T(", fullscreen") : _T(""));

      LOG_INFO(cszText, Log::Client::Renderer);
   }

   CStringA cszaWindowCaption(cszWindowCaption);
   SDL_Window* pWindow = SDL_CreateWindow(cszaWindowCaption.GetString(),
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      static_cast<int>(uiWidth),
      static_cast<int>(uiHeight),
      iFlags);

   if (pWindow == nullptr)
   {
      CString cszText;
      cszText.Format(_T("video mode set failed: %hs"), SDL_GetError());

      LOG_ERROR(cszText, Log::Client::Renderer);
      throw Exception(cszText, __FILE__, __LINE__);
   }

   m_spWindow.reset(pWindow, &SDL_DestroyWindow);

   SDL_GLContext glcontext = SDL_GL_CreateContext(pWindow);
   m_spGLCcontext.reset(glcontext, &SDL_GL_DeleteContext);

   // output some OpenGL diagnostics
   OpenGL::LogDiagnostics();

   ResizeView(uiWidth, uiHeight);
}

void RenderWindow::ResizeView(unsigned int uiWidth, unsigned int uiHeight)
{
   // setup OpenGL viewport
   glViewport(0, 0, static_cast<GLsizei>(uiWidth), static_cast<GLsizei>(uiHeight));
}
