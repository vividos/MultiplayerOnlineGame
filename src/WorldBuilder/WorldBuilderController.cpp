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

WorldBuilderController::~WorldBuilderController() throw()
{
}

BOOL WorldBuilderController::PreTranslateMessage(MSG* pMsg)
{
   switch (pMsg->message)
   {
   case WM_MOUSEMOVE:
      {
         CPoint pt(pMsg->lParam);
         CPoint ptRel(0, 0);

         if (m_bLastMousePosValid)
         {
            ptRel = m_ptLastMousePos - pt;
            CPoint m_ptLastMousePos;
         }

         m_spController->OnMouseMotionEvent(pt.x, pt.y, ptRel.x, ptRel.y);

         m_ptLastMousePos = pt;
         m_bLastMousePosValid = true;
      }
      break;

   case WM_MOUSELEAVE:
      m_bLastMousePosValid = false;
      break;

   case WM_LBUTTONDOWN:
   case WM_LBUTTONUP:
   case WM_RBUTTONDOWN:
   case WM_RBUTTONUP:
      {
         CPoint pt(pMsg->lParam);
         bool bDown = pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_RBUTTONDOWN;
         bool bLeft = pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONUP;

         T_enMouseButtonType enType = bLeft ? buttonLeft : buttonRight;
         m_spController->OnMouseButtonEvent(bDown, enType, pt.x, pt.y);
      }
      break;

   case WM_KEYDOWN:
   case WM_KEYUP:
      {
         bool bDown = pMsg->message == WM_KEYDOWN;
         unsigned int uiScancode = (pMsg->lParam >> 16) & 0xff; // bits 16-23

         unsigned int uiMod = 0;
         if (0 != ::GetKeyState(VK_MENU)) uiMod |= KMOD_ALT;
         if (0 != ::GetKeyState(VK_SHIFT)) uiMod |= KMOD_SHIFT;
         if (0 != ::GetKeyState(VK_CONTROL)) uiMod |= KMOD_CTRL;

         if (!m_keyboardActionManager.OnKeyboardEvent(bDown, uiScancode, uiMod))
            return FALSE;
      }
      break;

   default:
      return FALSE;
   }

   return TRUE; // message was handled
}

void WorldBuilderController::OnIdle()
{
   m_spController->Tick();
}
