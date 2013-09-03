//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file OverviewController.cpp Arena overview controller
//

// includes
#include "StdAfx.h"
#include "OverviewController.hpp"
#include "PerspectiveCamera.hpp"
#include "KeyboardActionManager.hpp"
#include <functional>

/// movement speed, in units per second
const double c_dDefaultMovementSpeed = 40.0;

OverviewController::OverviewController(KeyboardActionManager& keyboardActionManager,
   std::shared_ptr<PerspectiveCamera> spCamera)
:m_spCamera(spCamera),
 m_vPosition(spCamera->GetPosition()),
 m_dAngleDirection(spCamera->GetAngleDirection()),
 m_dAngleUp(spCamera->GetAngleUp()),
 m_dMovementSpeed(c_dDefaultMovementSpeed),
 m_bDragView(false),
 m_actionBindings(keyboardActionManager)
{
   // register keys
   RegisterKeys(keyboardActionManager);

   TimedEffect<OverviewController>::Init();
}

void OverviewController::Tick()
{
   TimedEffect<OverviewController>::Tick();
}

void OverviewController::RegisterKeys(KeyboardActionManager& mgr)
{
   mgr.RegisterActionKey(_T("up"),     KeyboardActionManager::actionOverviewMoveForward);
   mgr.RegisterActionKey(_T("left"),   KeyboardActionManager::actionOverviewMoveLeft);
   mgr.RegisterActionKey(_T("right"),  KeyboardActionManager::actionOverviewMoveRight);
   mgr.RegisterActionKey(_T("down"),   KeyboardActionManager::actionOverviewMoveBackward);

   // register some key actions
   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionOverviewMoveForward,
      std::bind(&OverviewController::SetForwardBackward, this, std::placeholders::_1, Vector3d(0.0, 0.0, 1.0)));
   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionOverviewMoveLeft,
      std::bind(&OverviewController::SetLeftRight, this, std::placeholders::_1, Vector3d(1.0, 0.0, 0.0)));
   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionOverviewMoveRight,
      std::bind(&OverviewController::SetLeftRight, this, std::placeholders::_1, Vector3d(-1.0, 0.0, 0.0)));
   m_actionBindings.RegisterActionHandler(KeyboardActionManager::actionOverviewMoveBackward,
      std::bind(&OverviewController::SetForwardBackward, this, std::placeholders::_1, Vector3d(0.0, 0.0, -1.0)));
}

void OverviewController::OnMouseButtonEvent(bool bPressed, T_enMouseButtonType enButtonType, int /*x*/, int /*y*/)
{
   if (enButtonType != buttonLeft)
      return;

   m_bDragView = bPressed;
}

void OverviewController::OnMouseMotionEvent(int /*x*/, int /*y*/, int xrel, int yrel)
{
   if (m_bDragView)
   {
      m_dAngleDirection += 0.1 * xrel;
      m_dAngleUp += 0.1 * yrel;
   }

   m_spCamera->SetPosition(m_vPosition, m_dAngleDirection, m_dAngleUp);
}

void OverviewController::OnTick(double dElapsed)
{
   // construct movement vector
   Vector3d vDir = m_vForwardBackward + m_vLeftRight;
   if (vDir.Length() == 0)
      return;

   vDir.RotateX(-m_dAngleUp);
   vDir.RotateY(180.0 - m_dAngleDirection);
   vDir.Normalize();
   vDir *= m_dMovementSpeed * dElapsed;

   m_vPosition += vDir;

   m_spCamera->SetPosition(m_vPosition, m_dAngleDirection, m_dAngleUp);
}

void OverviewController::SetForwardBackward(bool bPressed, Vector3d vDir)
{
   m_vForwardBackward = bPressed ? vDir : Vector3d();
}

void OverviewController::SetLeftRight(bool bPressed, Vector3d vDir)
{
   m_vLeftRight = bPressed ? vDir : Vector3d();
}
