//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file IAudioManager.hpp Audio subsystem interface
//
#pragma once

// includes
#include "Audio.hpp"

// forward references
class Vector3d;
namespace Stream
{
class IStream;
}

/// audio related classes
namespace Audio
{

/// represents a positional audio source
class ISource
{
public:
   /// dtor
   virtual ~ISource() throw() {}

   /// sets new position
   virtual void SetPosition(const Vector3d& vPos) = 0;

   /// starts playing sound
   virtual void Play(LPCTSTR pszSoundId, bool bLoop, bool bFadeIn) = 0;

   /// fades out currently playing sound
   virtual void Fadeout(double dFadeoutTime = 0.0) = 0;
};

/// source shared pointer
typedef std::shared_ptr<ISource> ISourcePtr;

/// interface for controlling audio playback
class IPlaybackControl
{
public:
   /// fades out current playback
   virtual void Fadeout(double dFadeoutTime = 0.0) = 0;
};

/// interface for controlling volume
class IVolumeControl
{
public:
   /// dtor
   virtual ~IVolumeControl() throw() {}

   /// volume type
   enum T_enVolumeType
   {
      enVolumeUserInterface,     ///< user interface sound
      enVolumeBackgroundMusic,   ///< background music
      enVolumePositional,        ///< positional sound sources
   };

   /// returns volume
   virtual float GetVolume(T_enVolumeType enVolumeType) = 0;

   /// sets new volume
   virtual void SetVolume(T_enVolumeType enVolumeType, float fValue) = 0;
};

/// \brief audio subsystem interface
class IAudioManager
{
public:
   /// dtor
   virtual ~IAudioManager() throw() {}

   /// returns audio manager instance
   static AUDIO_DECLSPEC IAudioManager& Get();

   /// returns audio io service
   virtual boost::asio::io_service& GetIoService() = 0;

   /// returns volume control
   virtual IVolumeControl& GetVolumeControl() = 0;

   /// creates a new positional audio source
   virtual std::shared_ptr<ISource> CreateSource() = 0;

   /// plays back sound, with given id; see AudioSoundType.hpp for ids
   virtual std::shared_ptr<IPlaybackControl> PlaySound(bool bUserInterface, LPCTSTR pszSoundId) = 0;

   /// plays back music, with given id and data stream
   virtual void PlayMusic(LPCTSTR pszMusicId, std::shared_ptr<Stream::IStream> spStream) = 0;
};

} // namespace Audio
