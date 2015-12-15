//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ModelViewerViewModel.cpp Model viewer view model
//

// includes
#include "stdafx.h"
#include "ModelViewerViewModel.hpp"
#include "MilkShape3D/AnimationData.hpp"
#include "MobileDisplayInfo.hpp"

using ModelViewer::ViewModel;

ViewModel::ViewModel()
:m_uiCurrentAnimation(3), // 1h_idle
m_player(ObjectId::New())
{
   MilkShape3D::WarriorsAndCommoners::AnimationData::EnumAnimations(m_vecAnimationNames);
}

void ViewModel::GetAnimationFrames(const CString& cszAnimationName,
   unsigned int& uiStartFrame, unsigned int& uiEndFrame)
{
   MilkShape3D::WarriorsAndCommoners::AnimationData::GetAnimationFrames(
      cszAnimationName, uiStartFrame, uiEndFrame);
}

void ViewModel::DisplayInfo(const MobileDisplayInfo& displayInfo) throw()
{
   m_player.DisplayInfo(displayInfo);

   ModelDescriptionChangedEvent().Call(m_player);
}

void ViewModel::CurrentAnimation(unsigned int uiCurrentAnimation) throw()
{
   ATLASSERT(uiCurrentAnimation < m_vecAnimationNames.size());
   m_uiCurrentAnimation = uiCurrentAnimation;

   m_animationChangedEvent.Call(uiCurrentAnimation);
}
