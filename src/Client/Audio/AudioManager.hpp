//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AudioManager.cpp Audio manager implementation
//
#pragma once

// includes
#include "IAudioManager.hpp"
#include "Singleton.hpp"
#include "OpenAL.hpp"
#include "NamedBufferMap.hpp"
#include "PositionalSource.hpp"
#include "IoServiceThread.hpp"
#include <set>

namespace Audio
{
// forward references
class PlaybackControl;

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

   /// registers a positional source
   void RegisterPositionalSource(OpenAL::SourcePtr spSource);

   /// unregisters a positional source
   void UnregisterPositionalSource(OpenAL::SourcePtr spSource);

   // virtual methods from IAudioManager

   /// returns audio io service
   virtual boost::asio::io_service& GetIoService() override
   {
      return m_ioServiceThread.Get();
   }

   /// adds sound id mapping
   virtual void AddSoundIdMapping(LPCTSTR pszSoundId, LPCTSTR pszRelativeFilename) override
   {
      m_mapSoundIds[pszSoundId] = pszRelativeFilename;
   }

   /// sets function to resolve file streams from ids
   virtual void SetFileStreamResolver(T_fnResolveFileStream fnResolveFileStream) override
   {
      m_fnResolveFileStream = fnResolveFileStream;
   }

   /// returns volume control
   virtual IVolumeControl& GetVolumeControl() override
   {
      return *this;
   }

   /// creates a new positional audio source
   virtual std::shared_ptr<IPositionalSource> CreateSource() override;

   /// plays back sound, with given id; see AudioSoundType.hpp for ids
   virtual std::shared_ptr<IPlaybackControl> PlaySound(LPCTSTR pszSoundId) override;

   /// plays back music, with given id and data stream
   virtual void PlayMusic(LPCTSTR pszMusicId) override;

   // virtual methods from IVolumeControl

   /// returns volume
   virtual float GetVolume(T_enVolumeType enVolumeType) const override;

   /// sets new volume
   virtual void SetVolume(T_enVolumeType enVolumeType, float fValue) override;

private:
   friend PositionalSource;

   // (re-)starts buffer map cleanup timer
   void RestartCleanupTimer();

   /// cleans buffer map
   void CleanupBufferMap(const boost::system::error_code& ec);

   /// plays sound
   std::shared_ptr<IPlaybackControl> StartPlay(OpenAL::SourcePtr spSource, const CString& cszSoundId,
      bool bLooping, bool bFadein);

   /// worker thread function; plays sound
   void AsyncPlay(OpenAL::SourcePtr spSource, const CString& cszSoundId,
      std::shared_ptr<PlaybackControl> spPlaybackControl, bool bLooping, bool bFadein);

   /// resolves stream from sound id
   OpenAL::BufferPtr ResolveBuffer(LPCTSTR pszSoundId);

   /// reads Ogg Vorbis audio file from stream into buffer
   OpenAL::BufferPtr ReadOggVorbisFile(std::shared_ptr<Stream::IStream> spStream) const;

private:
   /// audio background thread
   IoServiceThread m_ioServiceThread;

   /// current audio device
   OpenAL::Device m_audioDevice;

   /// source for background music playback
   OpenAL::SourcePtr m_spMusicSource;

   /// source for sound playback
   OpenAL::SourcePtr m_spSoundSource;

   /// all registered positional sources
   std::set<OpenAL::SourcePtr> m_setPositonalSources;

   /// positional volume value
   float m_fVolumePositional;

   /// mapping from sound ids to relative filenames
   std::map<CString, CString> m_mapSoundIds;

   /// function to resolve file streams from ids
   T_fnResolveFileStream m_fnResolveFileStream;

   /// named buffer map
   NamedBufferMap m_namedBufferMap;

   /// timer for cleaning up named buffer map
   boost::asio::deadline_timer m_timerCleanupBufferMap;
};

} // namespace Audio
