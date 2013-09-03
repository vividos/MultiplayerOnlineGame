//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file ClientLogic/GameClientBase.cpp Game client base class
//

// includes
#include "stdafx.h"
#include <SDL.h>
#include "GameClientBase.hpp"
#include "Scene.hpp"
#include "RenderEngine.hpp"
#include "WindowManager.hpp"

/// user code for event dispatch; see GameClientBase::DispatchInEventLoop()
const int c_iUserEventDispatch = 0;

GameClientBase::GameClientBase(const CString& cszClientName)
:MainGameLoop(true, cszClientName),
 m_actionBindings(m_keyboardActionManager),
 m_bIgnoreNextMouseMotion(false),
 m_iMouseMotionPosToIgnoreX(0),
 m_iMouseMotionPosToIgnoreY(0)
{
   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionQuitApplication,
      true, std::bind(&MainGameLoop::QuitLoop, this));

   //m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionToggleFullscreen,
   //   false, std::bind(&GameAppBase::ToggleFullscreen, this));


}

GameClientBase::~GameClientBase()
{
   SDL_Quit();
}

void GameClientBase::Init(unsigned int uiWidth, unsigned int uiHeight, bool bFullscreen)
{
   m_spWindowManager.reset();

   m_spRenderEngine.reset(new RenderEngine(uiWidth, uiHeight, bFullscreen));

   m_spWindowManager.reset(new WindowManager);
}

void GameClientBase::Run()
{
   MainGameLoop::Run();

   m_spScene.reset();

   m_spRenderEngine.reset();
}

RenderEngine& GameClientBase::GetRenderEngine()
{
   ATLASSERT(m_spRenderEngine != nullptr);
   return *m_spRenderEngine;
}

void GameClientBase::OnRender()
{
   if (m_spScene != nullptr)
      m_spScene->PreRender();

   if (m_spWindowManager != nullptr)
      m_spWindowManager->PreRender();

   if (m_spRenderEngine != nullptr)
      m_spRenderEngine->Render();

   // render UI
   if (m_spWindowManager != nullptr)
      m_spWindowManager->Render();

   if (m_spRenderEngine != nullptr)
      m_spRenderEngine->SwapBuffers();
}

void GameClientBase::OnEvent(SDL_Event& evt)
{
   switch (evt.type)
   {
   case SDL_MOUSEBUTTONDOWN:
      static_assert(SDL_BUTTON_LEFT      == buttonLeft, "must correspond to SDL definition");
      static_assert(SDL_BUTTON_MIDDLE    == buttonMiddle, "must correspond to SDL definition");
      static_assert(SDL_BUTTON_RIGHT     == buttonRight, "must correspond to SDL definition");
      static_assert(SDL_BUTTON_WHEELUP   == buttonWheelUp, "must correspond to SDL definition");
      static_assert(SDL_BUTTON_WHEELDOWN == buttonWheelDown, "must correspond to SDL definition");

      m_spScene->OnMouseButtonEvent(evt.button.state == SDL_PRESSED,
         static_cast<T_enMouseButtonType>(evt.button.button), evt.button.x, evt.button.y);
      break;

   case SDL_MOUSEMOTION:
      if (m_bIgnoreNextMouseMotion &&
            m_iMouseMotionPosToIgnoreX == evt.motion.x &&
            m_iMouseMotionPosToIgnoreY == evt.motion.y)
      {
         m_bIgnoreNextMouseMotion = false;
         break;
      }

      if (m_spWindowManager != nullptr)
         m_spWindowManager->OnMouseMotionEvent(evt.motion.x, evt.motion.y);

      m_spScene->OnMouseMotionEvent(evt.motion.x, evt.motion.y, evt.motion.xrel, evt.motion.yrel);
      break;

   case SDL_MOUSEBUTTONUP:
      {
         bool bHandled = false;

         if (m_spWindowManager != nullptr)
         {
            bHandled = m_spWindowManager->OnMouseButtonEvent(evt.button.state == SDL_PRESSED,
               static_cast<T_enMouseButtonType>(evt.button.button), evt.button.x, evt.button.y);
         }

         if (!bHandled)
         {
            m_spScene->OnMouseButtonEvent(evt.button.state == SDL_PRESSED,
               static_cast<T_enMouseButtonType>(evt.button.button), evt.button.x, evt.button.y);
         }
      }
      break;

   case SDL_KEYDOWN:
   case SDL_KEYUP:
      // handle key presses
      {
         bool bHandled = false;

         if (m_spWindowManager != nullptr)
         {
            // window manager gets first shot
            bHandled = m_spWindowManager->OnKeyboardEvent(evt.key.state == SDL_PRESSED, evt.key.keysym.sym, evt.key.keysym.mod);
         }
         
         if (!bHandled)
            m_keyboardActionManager.OnKeyboardEvent(evt.key.state == SDL_PRESSED, evt.key.keysym.sym, evt.key.keysym.mod);
      }
      break;

   case SDL_VIDEORESIZE:
      // TODO resize render window
      break;


   case SDL_USEREVENT:
      // handle user events
      switch (evt.user.code)
      {
      case c_iUserEventDispatch:
         {
            std::function<void()>* pfn =
               reinterpret_cast<std::function<void()>*>(evt.user.data1);
            (*pfn)(); // call function
            delete pfn;
         }
         break;

      default:
         ATLASSERT(false); // unknown user event code
         break;
      }
      break;

   default:
      MainGameLoop::OnEvent(evt);
      break;
   }

   // after each event, see if there's a previous scene to destroy
   if (m_spPreviousScene != NULL)
      m_spPreviousScene.reset();
}

void GameClientBase::ChangeScene(std::shared_ptr<Scene> spScene)
{
   m_spPreviousScene = m_spScene; // save scene here so that it can be reset when done processing events
   m_spScene = spScene;

   if (m_spScene == NULL)
      MainGameLoop::QuitLoop();
}

void GameClientBase::DispatchInEventLoop(std::function<void()> fn)
{
   SDL_Event userEvent = {0};
   userEvent.type = SDL_USEREVENT;
   userEvent.user.code = c_iUserEventDispatch;
   userEvent.user.data1 = new std::function<void()>(fn);

   ATLVERIFY(0 == SDL_PushEvent(&userEvent));
}

Point GameClientBase::GetMousePos() throw()
{
   int x = 0, y = 0;
   SDL_GetMouseState(&x, &y);
   return Point(x, y);
}

void GameClientBase::SetMousePos(const Point& pt) throw()
{
   // this call causes an event of type SDL_MOUSEMOTION ...
   SDL_WarpMouse(static_cast<Uint16>(pt.X()), static_cast<Uint16>(pt.Y()));

   // ... so ignore it
   m_bIgnoreNextMouseMotion = true;

   // also remember x and y positions to identify the message to ignore;
   // meanwhile some more mouse motion events could be caused by the user
   m_iMouseMotionPosToIgnoreX = pt.X();
   m_iMouseMotionPosToIgnoreY = pt.Y();
}

void GameClientBase::ShowCursor(bool bShow) throw()
{
   SDL_ShowCursor(bShow ? 1 : 0);
}

Size GameClientBase::GetScreenSize() const throw()
{
   SDL_Surface* surf = SDL_GetVideoSurface();
   return Size(surf->w, surf->h);
}
