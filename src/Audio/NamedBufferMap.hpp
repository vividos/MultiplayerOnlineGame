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
#include <ulib/DateTime.hpp>
#include <map>

namespace Audio
{

/// mapping from name to OpenAL buffer
class NamedBufferMap
{
public:
   /// returns if a buffer for given sound id is available
   bool IsAvailable(LPCTSTR pszSoundId) const;

   /// returns buffer, if available (or nullptr)
   OpenAL::BufferPtr GetBuffer(LPCTSTR pszSoundId) const;

   /// adds new named buffer
   void Add(LPCTSTR pszSoundId, OpenAL::BufferPtr spBuffer, bool bPermanent = false);

   /// cleans up buffer that were last accessed before given time span
   void Clean(TimeSpan tsOlderThan);

private:
   /// buffer entry
   struct BufferEntry
   {
      /// default ctor
      BufferEntry()
         :m_bPermanent(false)
      {
      }

      /// ctor
      BufferEntry(OpenAL::BufferPtr spBuffer, bool bPermanent)
         :m_spBuffer(spBuffer),
          m_bPermanent(bPermanent),
          m_dtLastAccess(DateTime::Now())
      {
      }

      /// sets last access date to now
      void UpdateAccessDate()
      {
         m_dtLastAccess = DateTime::Now();
      }

      /// audio buffer
      OpenAL::BufferPtr m_spBuffer;

      /// indicates if buffer is permanently loaded
      bool m_bPermanent;

      /// last access date of buffer
      DateTime m_dtLastAccess;
   };

   /// type for named buffer map
   typedef std::map<CString, BufferEntry> T_mapNamedBuffer;

   /// mapping
   T_mapNamedBuffer m_mapNamedBuffer;
};

} // namespace Audio
