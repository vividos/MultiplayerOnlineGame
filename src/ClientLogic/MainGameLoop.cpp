//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file MainGameLoop.cpp Main game loop
//

// includes
#include "StdAfx.h"
#include "MainGameLoop.hpp"
#include "FrameCounter.hpp"
#include "OpenGL.hpp"
#include <SDL.h>

/// user code for event dispatch; see MainGameLoop::DispatchInEventLoop()
const int c_iUserEventDispatch = 0;

MainGameLoop::MainGameLoop(bool bUpdateFrameCount, const CString& cszClientName)
:m_bUpdateFrameCount(bUpdateFrameCount),
 m_cszClientName(cszClientName),
 m_bExitLoop(false),
 m_bAppActive(true),
 m_bMouseFocus(true),
 m_bInputFocus(true)
{
}

void MainGameLoop::Run()
{
   FrameCounter fc;

   // main game loop
   while (!m_bExitLoop)
   {
      OnTick();

      ProcessEvents();
      if (m_bExitLoop)
         break;

      if (!m_bAppActive)
      {
         // as we're not visible, just wait for next event
         SDL_WaitEvent(NULL);
         continue;
      }

      // render the scene
      {
         OpenGL::ResetPolycount();

         OnRender();

         fc.OnNextFrame();
      }

      // count ticks for fps
      double dFramesPerSecond;
      if (m_bUpdateFrameCount && fc.GetUpdatedCount(dFramesPerSecond))
         UpdateCaption(dFramesPerSecond);
   }
}

void MainGameLoop::UpdateCaption(double dFramesPerSecond)
{
   unsigned int uiPolycount = OpenGL::Polycount();

   // set new caption
   CString cszCaption;
   cszCaption.Format(_T("%s; %3u.%01u frames/s, %u polygons"),
      m_cszClientName,
      unsigned(dFramesPerSecond),
      unsigned((dFramesPerSecond-unsigned(dFramesPerSecond))*10.0),
      uiPolycount);

   SDL_WM_SetCaption(CStringA(cszCaption), NULL);
}

void MainGameLoop::ProcessEvents()
{
   SDL_Event evt;

   // get another event
   while (SDL_PollEvent(&evt) && !m_bExitLoop)
   {
      switch (evt.type)
      {
      case SDL_QUIT:
         // handle quit requests
         m_bExitLoop = true;
         break;

      case SDL_ACTIVEEVENT:
         if (evt.active.state & SDL_APPACTIVE)
            m_bAppActive = evt.active.gain != 0; // application active? e.g. running vs. iconified

         if (evt.active.state & SDL_APPMOUSEFOCUS)
            m_bMouseFocus = evt.active.gain != 0; // mouse has focus in window?

         if (evt.active.state & SDL_APPMOUSEFOCUS)
            m_bInputFocus = evt.active.gain != 0; // app has input focus
         break;

      case SDL_VIDEOEXPOSE:
         // Screen needs to be redrawn; since we do that anyway
         // after processing all messages, just do nothing.
         break;

      default:
         OnEvent(evt);
         //ATLTRACE(_T("unknown event: %u\n"), evt.type);
         break;
      }
   }
}

void MainGameLoop::QuitLoop()
{
   m_bExitLoop = true;
}
