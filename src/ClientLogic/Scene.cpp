//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Scene.cpp Scene base class
//

// includes
#include "stdafx.h"
#include "Scene.hpp"
#include "IWindowManager.hpp"
#include "InstanceManager.hpp"
#include "IView.hpp"
#include "IViewController.hpp"

void Scene::PreRender()
{
   if (m_spView != nullptr)
      m_spView->PreRender();

   if (m_spController != nullptr)
      m_spController->Tick();
}

void Scene::OnMouseButtonEvent(bool bPressed, T_enMouseButtonType enButtonType, int x, int y)
{
   if (m_spView != nullptr)
      m_spView->OnMouseButtonEvent(bPressed, enButtonType, x, y);

   if (m_spController != NULL)
      m_spController->OnMouseButtonEvent(bPressed, enButtonType, x, y);
}

void Scene::OnMouseMotionEvent(int x, int y, int xrel, int yrel)
{
   if (m_spView != nullptr)
      m_spView->OnMouseMotionEvent(x, y, xrel, yrel);

   if (m_spController != nullptr)
      m_spController->OnMouseMotionEvent(x, y, xrel, yrel);
}
