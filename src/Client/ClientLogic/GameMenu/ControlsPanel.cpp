//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file GameMenu/ControlsPanel.cpp Game menu: Controls panel
//

// includes
#include "stdafx.h"
#include "ControlsPanel.hpp"
#include "IWindowManager.hpp"
#include "WindowManager.hpp"
#include "IFileSystem.hpp"
#include "Panel.hpp"
#include "Button.hpp"

using GameMenu::ControlsPanel;

ControlsPanel::ControlsPanel(IWindowManager& windowManager, IFileSystem& fileSystem, std::function<void()> fnOnReturnMainPanel)
:ISubPanel(fnOnReturnMainPanel)
{
   WindowManager::CreateFromJson(windowManager.GetRootWindow(), fileSystem, _T("ui/GameMenu/ControlsPanel.json"));

   m_spPanel = windowManager.FindWindow<Panel>(_T("panel"));
}

void ControlsPanel::Show(IWindowManager& windowManager)
{
   std::shared_ptr<Button> spBtnClose = windowManager.FindWindow<Button>(_T("btnClose"));
   ATLASSERT(spBtnClose != nullptr);

   spBtnClose->On(ButtonEvent::Clicked, m_fnOnReturnMainPanel);
}

void ControlsPanel::Hide()
{
   m_spPanel->Destroy();
}
