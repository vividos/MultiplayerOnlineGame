//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Source.hpp Audio source implementation
//
#pragma once

// includes
#include "OpenAL.hpp"
#include "IAudioManager.hpp"
#include <boost/enable_shared_from_this.hpp>

namespace Audio
{
class AudioManager;

/// audio source implementation
class Source: public ISource, public boost::enable_shared_from_this<Source>
{
public:
   /// ctor
   Source(AudioManager& audioManager, OpenAL::SourcePtr spSource)
      :m_spSource(spSource),
       m_audioManager(audioManager)
   {
   }

   /// sets new position
   virtual void SetPosition(const Vector3d& vPos)
   {
      m_spSource->Position(vPos);
   }

   /// sets new gain value
   void Gain(float fValue)
   {
      m_spSource->Gain(fValue);
   }

   /// returns source gain
   float Gain() const
   {
      return m_spSource->Gain();
   }

   /// starts playing sound
   virtual void Play(LPCTSTR pszSoundId, bool bLoop, bool bFadeIn) override;

   /// fades out currently playing sound
   virtual void Fadeout(double /*dFadeoutTime = 0.0*/)
   {
      // TODO impl
   }

   // normal methods

   /// starts playing a buffer
   void Play(OpenAL::BufferPtr spBuffer, bool bLoop, bool /*bFadeIn*/)
   {
      m_spSource->Stop();

      m_spSource->Attach(spBuffer);

      // start playing
      m_spSource->Looping(bLoop);

      m_spSource->Play();
   }

private:
   /// source
   OpenAL::SourcePtr m_spSource;

   /// audio manager instance
   AudioManager& m_audioManager;
};

} // namespace Audio
