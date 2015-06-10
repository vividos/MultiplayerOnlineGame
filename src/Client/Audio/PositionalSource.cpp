//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file PositionalSource.cpp Audio source implementation
//

// includes
#include "StdAfx.h"
#include "PositionalSource.hpp"
#include "AudioManager.hpp"

using Audio::PositionalSource;
using Audio::AudioManager;

PositionalSource::PositionalSource(AudioManager& audioManager, OpenAL::SourcePtr spSource)
:m_spSource(spSource),
 m_audioManager(audioManager)
{
   m_audioManager.RegisterPositionalSource(spSource);
}

PositionalSource::~PositionalSource()
{
   m_audioManager.UnregisterPositionalSource(m_spSource);
}

void PositionalSource::Play(LPCTSTR pszSoundId, bool bLoop, bool bFadeIn)
{
   m_audioManager.StartPlay(m_spSource, pszSoundId, bLoop, bFadeIn);
}
