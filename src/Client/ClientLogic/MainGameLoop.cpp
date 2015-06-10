//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MainGameLoop.cpp Main game loop
//

// includes
#include "StdAfx.h"
#include "MainGameLoop.hpp"
#include "FrameCounter.hpp"
#include "OpenGL.hpp"
#include <SDL.h>

/// user code for event dispatch; see MainGameLoop::DispatchInEventLoop()
const int c_iUserEventDispatch = 0;

MainGameLoop::MainGameLoop(bool bUpdateFrameCount, const CString& cszClientName,
   std::function<void(const CString&)> fnUpdateCaption)
:m_bUpdateFrameCount(bUpdateFrameCount),
 m_cszClientName(cszClientName),
 m_fnUpdateCaption(fnUpdateCaption),
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
   if (m_fnUpdateCaption == nullptr)
      return;

   unsigned int uiPolycount = OpenGL::Polycount();

   // set new caption
   CString cszCaption;
   cszCaption.Format(_T("%s; %3u.%01u frames/s, %u polygons"),
      m_cszClientName,
      unsigned(dFramesPerSecond),
      unsigned((dFramesPerSecond-unsigned(dFramesPerSecond))*10.0),
      uiPolycount);

   m_fnUpdateCaption(cszCaption);
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

      case SDL_APP_WILLENTERBACKGROUND:
         m_bAppActive = false;
         break;

      case SDL_APP_DIDENTERFOREGROUND:
         m_bAppActive = true;
         break;

      case SDL_WINDOWEVENT:
         switch (evt.window.event)
         {
         case SDL_WINDOWEVENT_ENTER:
            m_bAppActive = true;
            break;

         case SDL_WINDOWEVENT_LEAVE:
            m_bAppActive = false;
            break;

         default:
            break;
         }
         break;

      default:
         OnEvent(evt);
         break;
      }
   }
}

void MainGameLoop::QuitLoop()
{
   m_bExitLoop = true;
}
