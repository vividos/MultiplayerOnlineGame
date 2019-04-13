//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file WorldBuilderController.cpp World builder controller
//
#pragma once

// includes
#include "stdafx.h"
#include "WorldBuilderController.hpp"
#include "OverviewController.hpp"
#include <SDL.h>

WorldBuilderController::WorldBuilderController(std::shared_ptr<PerspectiveCamera> spCamera)
:m_spController(new OverviewController(m_keyboardActionManager, spCamera)),
 m_bLastMousePosValid(false)
{
   m_spController->SetMovementSpeed(200.0);
}

WorldBuilderController::~WorldBuilderController()
{
}

void WorldBuilderController::OnIdle()
{
   m_spController->Tick();

   ProcessEvents();
}

void WorldBuilderController::ProcessEvents()
{
   SDL_Event evt;

   // get another event
   while (SDL_PollEvent(&evt))
   {
      switch (evt.type)
      {
      case SDL_MOUSEBUTTONDOWN:
         static_assert(SDL_BUTTON_LEFT == buttonLeft, "must correspond to SDL definition");
         static_assert(SDL_BUTTON_MIDDLE == buttonMiddle, "must correspond to SDL definition");
         static_assert(SDL_BUTTON_RIGHT == buttonRight, "must correspond to SDL definition");

         m_spController->OnMouseButtonEvent(evt.button.state == SDL_PRESSED,
            static_cast<T_enMouseButtonType>(evt.button.button), evt.button.x, evt.button.y);
         break;

      case SDL_MOUSEWHEEL:
         m_spController->OnMouseWheelEvent(evt.wheel.x, evt.wheel.y);
         break;

      case SDL_MOUSEMOTION:
         m_spController->OnMouseMotionEvent(evt.motion.x, evt.motion.y, evt.motion.xrel, evt.motion.yrel);
         break;

      case SDL_MOUSEBUTTONUP:
         m_spController->OnMouseButtonEvent(evt.button.state == SDL_PRESSED,
            static_cast<T_enMouseButtonType>(evt.button.button), evt.button.x, evt.button.y);
         break;

      case SDL_KEYDOWN:
      case SDL_KEYUP:
         // handle key presses
         m_keyboardActionManager.OnKeyboardEvent(evt.key.state == SDL_PRESSED, evt.key.keysym.sym, evt.key.keysym.mod);
         break;
      }
   }
}
