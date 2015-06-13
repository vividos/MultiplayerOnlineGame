//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file UserInterfaceAudioManager.hpp UI audio manager
//
#pragma once

// includes
#include "ClientLogic.hpp"
#include "UserInterfaceAudio.hpp"

// forward references
namespace Audio
{
class IAudioManager;
}
class IWindowManager;
class IFileSystem;

/// manages audio for client
class CLIENTLOGIC_DECLSPEC UserInterfaceAudioManager: public boost::noncopyable
{
public:
   /// ctor
   UserInterfaceAudioManager(Audio::IAudioManager& audioManager);
   /// dtor
   ~UserInterfaceAudioManager() throw() {}

   /// connects window manager to audio manager
   void Connect(IWindowManager& windowManager, IFileSystem& fileSystem);

   /// called on user interface audio event
   void OnUserInterfaceAudioEvent(T_enUserInterfaceAudioEvents enAudioEvent) throw();

private:
   void LoadUserInterfaceSounds();

private:
   /// audio manager
   Audio::IAudioManager& m_audioManager;
};
