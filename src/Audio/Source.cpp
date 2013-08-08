//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Source.cpp Audio source implementation
//

// includes
#include "StdAfx.h"
#include "Source.hpp"
#include "AudioManager.hpp"

using Audio::Source;

void Source::Play(LPCTSTR pszSoundId, bool bLoop, bool bFadeIn)
{
   m_audioManager.AsyncPlay(shared_from_this(), pszSoundId, bLoop, bFadeIn);
}
