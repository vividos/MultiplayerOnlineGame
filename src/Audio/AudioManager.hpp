//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file AudioManager.cpp Audio manager implementation
//
#pragma once

// includes
#include "IAudioManager.hpp"
#include "Singleton.hpp"
#include "OpenAL.hpp"
#include "NamedBufferMap.hpp"
#include "Source.hpp"
#include "IoServiceThread.hpp"

namespace Audio
{

/// \brief Audio manager implementation
/// \details implements audio playback using OpenAL. The audio manager has
/// the following tasks:
/// * enumerate audio devices
/// * maintain a named sound map
/// * load sound files (*.wav and *.ogg) in the background
/// * play back 2D sound (i.e. UI sounds, player's steps)
/// * allocate a 3D audio source that can play sound positioned in a 3D world
/// There are three types of sound:
/// * Ambient music (soundtrack)
/// * UI sound
/// * 3D sound effects
/// Volume of all three can be controlled separately.
class AudioManager:
   public IAudioManager,
   public IVolumeControl,
   public Singleton<AudioManager>
{
public:
   /// ctor
   AudioManager();
   /// dtor
   virtual ~AudioManager() throw();

   // virtual methods from IAudioManager

   /// returns audio io service
   virtual boost::asio::io_service& GetIoService() override
   {
      return m_ioServiceThread.Get();
   }

   /// returns volume control
   virtual IVolumeControl& GetVolumeControl() override
   {
      return *this;
   }

   /// creates a new positional audio source
   virtual boost::shared_ptr<ISource> CreateSource() override
   {
      return boost::shared_ptr<ISource>(
         new Source(*this,
            m_audioDevice.CreateSource())
         );
   }

   /// plays back sound, with given id; see AudioSoundType.hpp for ids
   virtual boost::shared_ptr<IPlaybackControl> PlaySound(bool /*bUserInterface*/, LPCTSTR /*pszSoundId*/) override
   {
      // TODO impl
      return boost::shared_ptr<IPlaybackControl>();
   }

   /// plays back music, with given id and filename
   virtual void PlayMusic(LPCTSTR pszMusicId, LPCTSTR pszFilename) override;

   // virtual methods from IVolumeControl

   /// returns volume
   virtual float GetVolume(T_enVolumeType enVolumeType)
   {
      switch (enVolumeType)
      {
      case enVolumeUserInterface:
      case enVolumeBackgroundMusic:
      case enVolumePositional:
         return 1.0; // TODO impl
      }
      return 1.0; // TODO impl
   }

   /// sets new volume
   virtual void SetVolume(T_enVolumeType enVolumeType, float /*fValue*/)
   {
      switch (enVolumeType)
      {
      case enVolumeUserInterface:
         break;
      case enVolumeBackgroundMusic:
         break;
      case enVolumePositional:
         break;
      }
   }

   // methods

private:
   friend Source;

   void AsyncPlayMusic(const CString& cszMusicId, const CString& cszFilename);

   /// starts sound playback asynchronously
   void AsyncPlay(boost::shared_ptr<Source> spSource, LPCTSTR pszSoundId, bool bLoop, bool bFadeIn);

   void LoadAndPlay(boost::shared_ptr<Audio::Source> spSource, LPCTSTR pszSoundId, bool bLoop, bool bFadeIn);

   /// reads Ogg Vorbis audio file into buffer
   OpenAL::BufferPtr ReadOggVorbisFile(const CString& cszFilename) const;

private:
   /// current audio device
   OpenAL::Device m_audioDevice;

   /// source for background music playback
   boost::shared_ptr<Source> m_spMusicSource;

   /// named buffer map
   NamedBufferMap m_namedBufferMap;

   /// audio background thread
   IoServiceThread m_ioServiceThread;
};

} // namespace Audio
