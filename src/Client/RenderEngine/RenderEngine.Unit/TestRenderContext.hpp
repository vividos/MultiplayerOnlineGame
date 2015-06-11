//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file TestRenderContext.hpp Render context for unit tests
//
#pragma once

// includes
#include <SDL.h>
#include "OpenGL.hpp"
#include "RenderWindow.hpp"

/// render context that creates a window to have an OpenGL context
class TestRenderContext
{
public:
   /// ctor; creates the render window (and context)
   TestRenderContext()
   {
      if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
         throw std::runtime_error("SDL_Init() failed");

      m_spWindow.reset(new RenderWindow(_T("TestRenderContext"), 800, 600, false));

      glViewport(0, 0, 800, 600);
   }

   /// dtor; destroys the window again
   ~TestRenderContext()
   {
      m_spWindow.reset();

      SDL_QuitSubSystem(SDL_INIT_VIDEO);
   }

   /// swaps foreground and background buffers
   void SwapBuffers()
   {
      ATLASSERT(m_spWindow != nullptr);
      m_spWindow->SwapBuffers();
   }

private:
   /// render window
   std::shared_ptr<RenderWindow> m_spWindow;
};
