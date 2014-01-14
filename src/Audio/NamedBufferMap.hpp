//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file NamedBufferMap.hpp Named audio buffer mapping
//
#pragma once

// includes
#include "IAudioManager.hpp"
#include "OpenAL.hpp"
#include <map>

namespace Audio
{

/// mapping from name to OpenAL buffer
class NamedBufferMap
{
public:
   /// returns buffer, if available (or NULL)
   OpenAL::BufferPtr GetBuffer(LPCTSTR pszSoundId);

   /// adds new named buffer
   void Add(LPCTSTR pszSoundId, OpenAL::BufferPtr spBuffer);

private:
   /// mapping
   std::map<CString, OpenAL::BufferPtr> m_mapNamedBuffer;
};

} // namespace Audio
