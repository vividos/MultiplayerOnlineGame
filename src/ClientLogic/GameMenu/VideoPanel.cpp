//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file GameMenu/VideoPanel.cpp Game menu: Video panel
//

// includes
#include "stdafx.h"
#include "VideoPanel.hpp"
#include "IWindowManager.hpp"
#include "WindowManager.hpp"
#include "IFileSystem.hpp"
#include "Panel.hpp"
#include "Button.hpp"

using GameMenu::VideoPanel;

VideoPanel::VideoPanel(IWindowManager& windowManager, IFileSystem& fileSystem, std::function<void()> fnOnReturnMainPanel)
:ISubPanel(fnOnReturnMainPanel)
{
   WindowManager::CreateFromJson(windowManager.GetRootWindow(), fileSystem, _T("ui/GameMenu/VideoPanel.json"));

   m_spPanel = windowManager.FindWindow<Panel>(_T("panel"));
}

void VideoPanel::Show(IWindowManager& windowManager)
{
   std::shared_ptr<Button> spBtnClose = windowManager.FindWindow<Button>(_T("btnClose"));
   ATLASSERT(spBtnClose != nullptr);

   spBtnClose->On(ButtonEvent::Clicked, m_fnOnReturnMainPanel);
}

void VideoPanel::Hide()
{
   m_spPanel->Destroy();
}
