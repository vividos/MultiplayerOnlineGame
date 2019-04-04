//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ClientLogic/GameClientBase.cpp Game client base class
//

// includes
#include "stdafx.h"
#include <SDL.h>
#include "GameClientBase.hpp"
#include "Scene.hpp"
#include "RenderEngine.hpp"
#include "RenderWindow.hpp"
#include "WindowManager.hpp"

/// user code for event dispatch; see GameClientBase::DispatchInEventLoop()
const int c_iUserEventDispatch = 0;

GameClientBase::GameClientBase(const CString& cszClientName)
:MainGameLoop(true, cszClientName, std::bind(&GameClientBase::UpdateCaption, this, std::placeholders::_1)),
 m_cszClientName(cszClientName),
 m_bFullscreen(false),
 m_actionBindings(m_keyboardActionManager),
 m_bIgnoreNextMouseMotion(false),
 m_iMouseMotionPosToIgnoreX(0),
 m_iMouseMotionPosToIgnoreY(0)
{
   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionQuitApplication,
      true, std::bind(&MainGameLoop::QuitLoop, this));

   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionToggleFullscreen,
      false, std::bind(&GameClientBase::ToggleFullscreen, this));
}

GameClientBase::~GameClientBase()
{
   SDL_Quit();
}

void GameClientBase::Init(unsigned int uiWidth, unsigned int uiHeight, bool bFullscreen)
{
   m_bFullscreen = bFullscreen;

   m_spWindowManager.reset();

   m_spRenderEngine.reset(new RenderEngine(m_cszClientName, uiWidth, uiHeight, bFullscreen));

   m_spWindowManager.reset(new WindowManager(uiWidth, uiHeight));
}

void GameClientBase::Run()
{
   MainGameLoop::Run();

   m_graphicsTaskManager.Cancel();
   m_graphicsTaskManager.Join();

   m_spScene.reset();

   m_spRenderEngine.reset();
}

RenderEngine& GameClientBase::GetRenderEngine()
{
   ATLASSERT(m_spRenderEngine != nullptr);
   return *m_spRenderEngine;
}

IWindowManager& GameClientBase::GetWindowManager()
{
   ATLASSERT(m_spWindowManager != nullptr);
   return *m_spWindowManager;
}

void GameClientBase::OnRender()
{
   m_graphicsTaskManager.UploadOne();

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
   static_assert(SDL_BUTTON_LEFT      == buttonLeft, "must correspond to SDL definition");
   static_assert(SDL_BUTTON_MIDDLE    == buttonMiddle, "must correspond to SDL definition");
   static_assert(SDL_BUTTON_RIGHT     == buttonRight, "must correspond to SDL definition");

   switch (evt.type)
   {
   case SDL_MOUSEBUTTONDOWN:
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

   case SDL_MOUSEWHEEL:
      if (m_spWindowManager != nullptr)
         m_spWindowManager->OnMouseMotionEvent(evt.wheel.x, evt.wheel.y);

      m_spScene->OnMouseWheelEvent(evt.wheel.x, evt.wheel.y);
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

   case SDL_WINDOWEVENT:
      switch (evt.window.event)
      {
      case SDL_WINDOWEVENT_RESIZED:
         {
            Size size = GetScreenSize();

            if (m_spRenderEngine != nullptr)
            {
               m_spRenderEngine->GetRenderWindow().ResizeView(size.Width(), size.Height());
            }

            // set new size
            if (m_spWindowManager != nullptr)
            {
               m_spWindowManager->OnResizeScreen(size);
            }
         }
         break;
      }
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

   ATLVERIFY(1 == SDL_PushEvent(&userEvent));
}

Point GameClientBase::GetMousePos()
{
   int x = 0, y = 0;
   SDL_GetMouseState(&x, &y);
   return Point(x, y);
}

void GameClientBase::SetMousePos(const Point& pt)
{
   // this call causes an event of type SDL_MOUSEMOTION ...
   if (m_spRenderEngine != nullptr)
      m_spRenderEngine->GetRenderWindow().SetMousePos(pt.X(), pt.Y());

   // ... so ignore it
   m_bIgnoreNextMouseMotion = true;

   // also remember x and y positions to identify the message to ignore;
   // meanwhile some more mouse motion events could be caused by the user
   m_iMouseMotionPosToIgnoreX = pt.X();
   m_iMouseMotionPosToIgnoreY = pt.Y();
}

void GameClientBase::ShowCursor(bool bShow)
{
   SDL_ShowCursor(bShow ? 1 : 0);
}

Size GameClientBase::GetScreenSize() const throw()
{
   int iWidth = 0, iHeight = 0;

   if (m_spRenderEngine != nullptr)
      m_spRenderEngine->GetRenderWindow().GetWindowSize(iWidth, iHeight);

   return Size(iWidth, iHeight);
}

void GameClientBase::UpdateCaption(const CString& cszCaption)
{
   if (m_spRenderEngine != nullptr)
      m_spRenderEngine->GetRenderWindow().SetCaption(cszCaption);
}

void GameClientBase::ToggleFullscreen()
{
   m_bFullscreen = !m_bFullscreen;

   if (m_spRenderEngine != nullptr)
      m_spRenderEngine->GetRenderWindow().SetFullscreen(m_bFullscreen);

   // set new size
   if (m_spWindowManager != nullptr)
   {
      Size size = GetScreenSize();
      m_spWindowManager->OnResizeScreen(size);
   }
}
