//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ModelViewerPanel.cpp Model viewer panel
//

// includes
#include "StdAfx.h"
#include "ModelViewerPanel.hpp"
#include "IWindowManager.hpp"
#include "WindowManager.hpp"
#include "TextField.hpp"
#include "Button.hpp"
#include "Panel.hpp"

ModelViewer::Panel::Panel(IWindowManager& windowManager, IFileSystem& fileSystem)
{
   WindowManager::CreateFromJson(windowManager.GetRootWindow(), fileSystem, _T("ui/ModelViewerPanel.json"));

   m_spPanel = windowManager.FindWindow<::Panel>(_T("mainPanel"));
}

void ModelViewer::Panel::Show(IWindowManager& windowManager)
{
   m_spPanel->Create(*windowManager.GetRootWindow());

   std::shared_ptr<Button> spBtnPrevAnim = windowManager.FindWindow<Button>(_T("btnPrevAnim"));
   ATLASSERT(spBtnPrevAnim != nullptr);

   spBtnPrevAnim->On(ButtonEvent::Clicked,
      std::bind(&Panel::OnButtonPrevNext, this, false));

   std::shared_ptr<Button> spBtnNextAnim = windowManager.FindWindow<Button>(_T("btnNextAnim"));
   ATLASSERT(spBtnNextAnim != nullptr);

   spBtnNextAnim->On(ButtonEvent::Clicked,
      std::bind(&Panel::OnButtonPrevNext, this, true));
}

void ModelViewer::Panel::AnimationName(const CString& cszAnimationName)
{
   std::shared_ptr<TextField> spTextField =
      m_spPanel->GetWindowManager().FindWindow<TextField>(_T("animationName"));
   ATLASSERT(spTextField != nullptr);

   spTextField->SetAttr(TextFieldAttr::Text, cszAnimationName);
}

void ModelViewer::Panel::OnButtonPrevNext(bool bDirectionNext)
{
   if (m_fnOnChangeAnimation)
      m_fnOnChangeAnimation(bDirectionNext);
}
