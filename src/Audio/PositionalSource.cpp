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

void PositionalSource::Play(LPCTSTR pszSoundId, bool bLoop, bool bFadeIn)
{
   // TODO start in worker thread
   m_audioManager.AsyncPlay(m_spSource, pszSoundId, bLoop, bFadeIn);
}
