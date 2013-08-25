//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file RenderWindow.cpp Render engine class
//

// includes
#include "StdAfx.h"
#include "RenderWindow.hpp"
#include <SDL.h>
#include "OpenGL.hpp"
#include "RenderOptions.hpp"

RenderWindow::RenderWindow(unsigned int uiWidth, unsigned int uiHeight, bool bFullscreen, RenderOptions& renderOptions)
{
   InitVideo(uiWidth, uiHeight, bFullscreen);
   SetupView(renderOptions);

   // call IsExtensionSupported here so that it gets initialized
   OpenGL::IsExtensionSupported(_T(""));

   // set our thread as render thread
   OpenGL::SetRenderThreadId();
}

void RenderWindow::SwapBuffers()
{
   ATLASSERT(OpenGL::IsRenderThread());

   SDL_GL_SwapBuffers();
}

void RenderWindow::SetFullscreen(bool bFullscreen)
{
   SDL_Surface* surf = SDL_GetVideoSurface();

   Uint32 flags = surf->flags;

   if (bFullscreen)
      flags |= SDL_FULLSCREEN;
   else
      flags &= ~SDL_FULLSCREEN;

   SDL_SetVideoMode(surf->w, surf->h, surf->format->BitsPerPixel, flags);
}

void RenderWindow::InitVideo(unsigned int uiWidth, unsigned int uiHeight, bool bFullscreen)
{
   // output SDL version number
   {
      const SDL_version* pVer = SDL_Linked_Version();
      CString cszText;
      cszText.Format(_T("using SDL %u.%u.%u"), pVer->major, pVer->minor, pVer->patch);
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

   // Information about the current video settings
   const SDL_VideoInfo* info = SDL_GetVideoInfo();

   if (!info)
   {
      // this should probably never happen
      CString cszText;
      cszText.Format(_T("video query failed: %hs"), SDL_GetError());

      LOG_ERROR(cszText, Log::Client::Renderer);
      throw Exception(cszText, __FILE__, __LINE__);
   }

   // print video driver stats
   {
      char buffer[256];
      SDL_VideoDriverName(buffer,256);

      CString cszText;
      cszText.Format(_T("video driver: %hs, ram available: %u kb"),
         buffer, info->video_mem);

      LOG_INFO(cszText, Log::Client::Renderer);
   }

   // set window caption
   // TODO remove, do in application class
   //SDL_WM_SetCaption("Client", NULL);
   SDL_WM_SetCaption("", NULL);

   // set up OpenGL attributes
   SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
   SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
   SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

   // setup video mode
   int bpp = info->vfmt->BitsPerPixel;
   int iFlags = SDL_OPENGL | SDL_HWSURFACE | SDL_OPENGLBLIT |
      (bFullscreen ? SDL_FULLSCREEN : 0);

   {
      CString cszText;
      cszText.Format(_T("setting video mode: %u x %u, %u bits%s"),
         uiWidth, uiHeight, bpp,
         bFullscreen ? _T(", fullscreen") : _T(""));

      LOG_INFO(cszText, Log::Client::Renderer);
   }

   if (SDL_SetVideoMode(static_cast<int>(uiWidth), static_cast<int>(uiHeight), bpp, iFlags) == 0)
   {
      CString cszText;
      cszText.Format(_T("video mode set failed: %hs"), SDL_GetError());

      LOG_ERROR(cszText, Log::Client::Renderer);
      throw Exception(cszText, __FILE__, __LINE__);
   }

   // output some OpenGL diagnostics
   OpenGL::LogDiagnostics();

   ResizeView(uiWidth, uiHeight);
}

void RenderWindow::SetupView(RenderOptions& renderOptions)
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
      glCullFace(GL_FRONT_AND_BACK);
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

void RenderWindow::ResizeView(unsigned int uiWidth, unsigned int uiHeight)
{
   // setup OpenGL viewport
   glViewport(0, 0, static_cast<GLsizei>(uiWidth), static_cast<GLsizei>(uiHeight));
}
