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

bool NamedBufferMap::IsAvailable(LPCTSTR pszSoundId) const
{
   return m_mapNamedBuffer.find(pszSoundId) != m_mapNamedBuffer.end();
}

OpenAL::BufferPtr NamedBufferMap::GetBuffer(LPCTSTR pszSoundId) const
{
   if (!IsAvailable(pszSoundId))
      return OpenAL::BufferPtr();

   T_mapNamedBuffer::const_iterator iter = m_mapNamedBuffer.find(pszSoundId);
   ATLASSERT(iter != m_mapNamedBuffer.end());

   return iter->second.m_spBuffer;
}

void NamedBufferMap::Add(LPCTSTR pszSoundId, OpenAL::BufferPtr spBuffer, bool bPermanent)
{
   if (!IsAvailable(pszSoundId))
      m_mapNamedBuffer[pszSoundId] = BufferEntry(spBuffer, bPermanent);
   else
      m_mapNamedBuffer[pszSoundId].UpdateAccessDate();
}
