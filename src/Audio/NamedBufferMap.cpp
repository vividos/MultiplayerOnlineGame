//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file NamedBufferMap.cpp Named audio buffer mapping
//

// includes
#include "StdAfx.h"
#include "NamedBufferMap.hpp"
#include "LogCategories.hpp"

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
   {
      LOG_INFO(_T("NamedBufferMap: adding buffer for sound id: ") + CString(pszSoundId), Log::Client::Audio);
      m_mapNamedBuffer[pszSoundId] = BufferEntry(spBuffer, bPermanent);
   }
   else
      m_mapNamedBuffer[pszSoundId].UpdateAccessDate();
}

void NamedBufferMap::Clean(TimeSpan tsOlderThan)
{
   if (m_mapNamedBuffer.empty())
      return;

   DateTime dtNewestDate = DateTime::Now() - tsOlderThan;

	for (auto iter = m_mapNamedBuffer.begin(), stop = m_mapNamedBuffer.end(); iter != stop;)
   {
      const BufferEntry& entry = iter->second;
         
      if (entry.m_bPermanent)
      {
            ++iter;
            continue;
      }

      if (entry.m_dtLastAccess < dtNewestDate)
      {
         CString cszSoundId = iter->first;
         LOG_INFO(_T("NamedBufferMap: removing buffer for sound id: ") + cszSoundId, Log::Client::Audio);

         ++iter;
         m_mapNamedBuffer.erase(cszSoundId);
      }
      else
         ++iter;
   }
}
