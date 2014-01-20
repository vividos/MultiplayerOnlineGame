//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file UserInterfaceAudioManager.cpp UI audio manager
//

// includes
#include "StdAfx.h"
#include "UserInterfaceAudioManager.hpp"
#include "IAudioManager.hpp"
#include "IWindowManager.hpp"
#include "AudioSoundType.hpp"

UserInterfaceAudioManager::UserInterfaceAudioManager(Audio::IAudioManager& audioManager)
:m_audioManager(audioManager)
{
   LoadUserInterfaceSounds();
}

void UserInterfaceAudioManager::Connect(IWindowManager& windowManager)
{
   windowManager.SetAudioEventHandler(
      std::bind(&UserInterfaceAudioManager::OnUserInterfaceAudioEvent, this, std::placeholders::_1));
}

void UserInterfaceAudioManager::OnUserInterfaceAudioEvent(T_enUserInterfaceAudioEvents enAudioEvent) throw()
{
   switch (enAudioEvent)
   {
   case uiButtonPress:
      m_audioManager.PlaySound(Audio::Sound::UI::ButtonPress);
      break;
   case uiButtonLeave:
      m_audioManager.PlaySound(Audio::Sound::UI::ButtonPress);
      break;
   case uiItemSelect:
      m_audioManager.PlaySound(Audio::Sound::UI::ItemSelected);
      break;

   case uiDialogOpenModal:
      m_audioManager.PlaySound(Audio::Sound::UI::Open);
      break;

   case uiDialogOpenNonmodal:
      m_audioManager.PlaySound(Audio::Sound::UI::Open);
      break;

   case uiDialogCloseModal:
      m_audioManager.PlaySound(Audio::Sound::UI::Close);
      break;

   case uiDialogCloseNonmodal:
      m_audioManager.PlaySound(Audio::Sound::UI::Close);
      break;

   case uiTooltipOpened:
      m_audioManager.PlaySound(Audio::Sound::UI::Open);
      break;

   case uiScrollOpen:
      m_audioManager.PlaySound(Audio::Sound::UI::Open);
      break;

   case uiScrollClosed:
      m_audioManager.PlaySound(Audio::Sound::UI::Close);
      break;
   }
}

void UserInterfaceAudioManager::LoadUserInterfaceSounds()
{
   // TODO implement
}
