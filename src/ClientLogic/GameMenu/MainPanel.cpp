//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file GameMenu/MainPanel.cpp Game menu: Main menu panel
//

// includes
#include "stdafx.h"
#include "MainPanel.hpp"
#include "IWindowManager.hpp"
#include "WindowManager.hpp"
#include "Panel.hpp"
#include "Button.hpp"
#include "ISubPanel.hpp"
#include "VideoPanel.hpp"
#include "AudioPanel.hpp"
#include "ControlsPanel.hpp"
#include "CreditsPanel.hpp"

using GameMenu::MainPanel;

MainPanel::MainPanel(IWindowManager& windowManager, IFileSystem& fileSystem)
:m_windowManager(windowManager),
m_fileSystem(fileSystem),
m_bShown(true)
{
   WindowManager::CreateFromJson(windowManager.GetRootWindow(), fileSystem, _T("ui/GameMenu/MainPanel.json"));

   m_spPanel = windowManager.FindWindow<Panel>(_T("mainPanel"));
}

void MainPanel::Show(IWindowManager& windowManager)
{
   if (!m_bShown)
   {
      m_spPanel->Create(*windowManager.GetRootWindow());
      m_bShown = true;
   }

   std::shared_ptr<Button> spBtnVideo = windowManager.FindWindow<Button>(_T("btnVideo"));
   std::shared_ptr<Button> spBtnAudio = windowManager.FindWindow<Button>(_T("btnAudio"));
   std::shared_ptr<Button> spBtnControls = windowManager.FindWindow<Button>(_T("btnControls"));
   std::shared_ptr<Button> spBtnCredits = windowManager.FindWindow<Button>(_T("btnCredits"));
   std::shared_ptr<Button> spBtnExit = windowManager.FindWindow<Button>(_T("btnExit"));
   std::shared_ptr<Button> spBtnReturn = windowManager.FindWindow<Button>(_T("btnReturn"));

   ATLASSERT(spBtnVideo != nullptr);
   ATLASSERT(spBtnAudio != nullptr);
   ATLASSERT(spBtnControls != nullptr);
   ATLASSERT(spBtnCredits != nullptr);
   ATLASSERT(spBtnExit != nullptr);
   ATLASSERT(spBtnReturn != nullptr);

   spBtnVideo->On(ButtonEvent::Clicked, std::bind(&MainPanel::OnButtonPressed, this, buttonVideo));
   spBtnAudio->On(ButtonEvent::Clicked, std::bind(&MainPanel::OnButtonPressed, this, buttonAudio));
   spBtnControls->On(ButtonEvent::Clicked, std::bind(&MainPanel::OnButtonPressed, this, buttonControls));
   spBtnCredits->On(ButtonEvent::Clicked, std::bind(&MainPanel::OnButtonPressed, this, buttonCredits));
   spBtnExit->On(ButtonEvent::Clicked, std::bind(&MainPanel::OnButtonPressed, this, buttonExit));
   spBtnReturn->On(ButtonEvent::Clicked, std::bind(&MainPanel::OnButtonPressed, this, buttonReturn));
}

void MainPanel::Hide()
{
   if (m_spPanel != nullptr)
      m_spPanel->Destroy();

   m_bShown = false;

   if (m_spSubPanel != nullptr)
      m_spSubPanel->Hide();
}

void MainPanel::OnButtonPressed(T_enButtonType enButtonType)
{
   std::shared_ptr<ISubPanel> spSubPanel;

   std::function<void()> fnOnReturnMainPanel =
      std::bind(&MainPanel::OnReturnMainPanel, this);

   switch (enButtonType)
   {
   case buttonVideo:    spSubPanel.reset(new VideoPanel(m_windowManager, m_fileSystem, fnOnReturnMainPanel)); break;
   case buttonAudio:    spSubPanel.reset(new AudioPanel(m_windowManager, m_fileSystem, fnOnReturnMainPanel)); break;
   case buttonControls: spSubPanel.reset(new ControlsPanel(m_windowManager, m_fileSystem, fnOnReturnMainPanel)); break;
   case buttonCredits:  spSubPanel.reset(new CreditsPanel(m_windowManager, m_fileSystem, fnOnReturnMainPanel)); break;

   case buttonExit:
      Hide();
      if (m_fnOnExitClicked)
         m_fnOnExitClicked();
      break;

   case buttonReturn:
      Hide();
      break;

   default:
      ATLASSERT(false);
      break;
   }

   if (spSubPanel != nullptr)
   {
      Hide();

      spSubPanel->Show(m_windowManager);
      m_spPanel->Destroy();
      m_bShown = false;

      m_spSubPanel = spSubPanel;
   }
}

void MainPanel::OnReturnMainPanel()
{
   if (m_spSubPanel != nullptr)
   {
      m_spSubPanel->Hide();
      m_spSubPanel.reset();
   }

   Show(m_windowManager);
}
