//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file NamedBufferMap.cpp Named audio buffer mapping
//

// includes
#include "StdAfx.h"
#include "NamedBufferMap.hpp"

using Audio::NamedBufferMap;

OpenAL::BufferPtr NamedBufferMap::GetBuffer(LPCTSTR /*pszSoundId*/)
{
   // TODO implement
   return OpenAL::BufferPtr();
}

void NamedBufferMap::Add(LPCTSTR /*pszSoundId*/, OpenAL::BufferPtr /*spBuffer*/)
{
}
