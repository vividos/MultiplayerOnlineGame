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
#include "IFileSystem.hpp"
#include "AudioSoundType.hpp"

UserInterfaceAudioManager::UserInterfaceAudioManager(Audio::IAudioManager& audioManager)
:m_audioManager(audioManager)
{
   LoadUserInterfaceSounds();
}

void UserInterfaceAudioManager::Connect(IWindowManager& windowManager, IFileSystem& fileSystem)
{
   windowManager.SetAudioEventHandler(
      std::bind(&UserInterfaceAudioManager::OnUserInterfaceAudioEvent, this, std::placeholders::_1));

   m_audioManager.SetFileStreamResolver(
      std::bind(&IFileSystem::OpenFile,
      std::ref(fileSystem),
      std::placeholders::_1,
      true));
}

void UserInterfaceAudioManager::OnUserInterfaceAudioEvent(T_enUserInterfaceAudioEvents enAudioEvent) throw()
{
   switch (enAudioEvent)
   {
   case uiButtonPress:
      m_audioManager.PlaySound(Audio::Sound::UI::ButtonPress);
      break;

   case uiItemSelect:
      m_audioManager.PlaySound(Audio::Sound::UI::ItemSelected);
      break;

   case uiDialogOpen:
      m_audioManager.PlaySound(Audio::Sound::UI::DialogOpen);
      break;

   case uiDialogClose:
      m_audioManager.PlaySound(Audio::Sound::UI::DialogClose);
      break;

   default:
      ATLASSERT(false);
      break;
   }
}

/// \note we only load static sounds here; provide a config loaded when needed
void UserInterfaceAudioManager::LoadUserInterfaceSounds()
{
   m_audioManager.AddSoundIdMapping(Audio::Sound::UI::ButtonPress, _T("audio/ui/button-press.ogg"));
   m_audioManager.AddSoundIdMapping(Audio::Sound::UI::ItemSelected, _T("audio/ui/item-selected.ogg"));
   m_audioManager.AddSoundIdMapping(Audio::Sound::UI::DialogOpen, _T("audio/ui/dialog-open.ogg"));
   m_audioManager.AddSoundIdMapping(Audio::Sound::UI::DialogClose, _T("audio/ui/dialog-close.ogg"));
}
