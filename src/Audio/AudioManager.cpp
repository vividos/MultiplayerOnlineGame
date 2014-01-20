//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AudioManager.hpp Audio manager implementation
//

// includes
#include "StdAfx.h"
#include "AudioManager.hpp"
#include "LogCategories.hpp"
#include "Timer.hpp"
#include "OggVorbisFileReader.hpp"
#include <ulib/stream/FileStream.hpp>
#include "Filesystem.hpp"

using namespace Audio;

/// elapsed time after which buffer map cleanup is done
const int c_iBufferMapCleanupElapsedTimeInSeconds = 5 * 60;

IMPLEMENT_SINGLETON(AudioManager)

//
// IAudioManager
//

IAudioManager& IAudioManager::Get()
{
   return AudioManager::Instance();
}


//
// PlaybackControl
//

/// playback control implementation
class Audio::PlaybackControl : public IPlaybackControl
{
public:
   /// ctor
   PlaybackControl(OpenAL::SourcePtr spSource)
      :m_spSource(spSource)
   {
   }

   /// fades out current playback
   virtual void Fadeout(double /*dFadeoutTime*/ = 0.0) override
   {
       // TODO
   }

private:
   /// source
   OpenAL::SourcePtr m_spSource;
};


//
// AudioManager
//

AudioManager::AudioManager()
:m_ioServiceThread(true, _T("Audio Subsystem Thread")), // has default work
 m_fVolumePositional(1.0f),
 m_timerCleanupBufferMap(m_ioServiceThread.Get())
{
   LOG_INFO(_T("starting audio subsystem"), Log::Client::Audio);

   // output version
   CString cszText;
   cszText.Format(_T("using OpenAL %s, vendor [%s], renderer [%s]"),
      m_audioDevice.Version(),
      m_audioDevice.Vendor(),
      m_audioDevice.Renderer()
      );
   LOG_INFO(cszText, Log::Client::Audio);

   m_ioServiceThread.Run();

   m_spSoundSource = m_audioDevice.CreateSource();
   m_spMusicSource = m_audioDevice.CreateSource();

   RestartCleanupTimer();
}

AudioManager::~AudioManager() throw()
{
   m_timerCleanupBufferMap.cancel();

   m_ioServiceThread.Get().stop();
   m_ioServiceThread.Join();

   LOG_INFO(_T("stopped audio subsystem"), Log::Client::Audio);
}

void AudioManager::RegisterPositionalSource(OpenAL::SourcePtr spSource)
{
   m_setPositonalSources.insert(spSource);
}

void AudioManager::UnregisterPositionalSource(OpenAL::SourcePtr spSource)
{
   m_setPositonalSources.erase(spSource);
}

std::shared_ptr<IPositionalSource> AudioManager::CreateSource()
{
   return std::shared_ptr<IPositionalSource>(
      new PositionalSource(*this,
         m_audioDevice.CreateSource())
      );
}

std::shared_ptr<IPlaybackControl> AudioManager::PlaySound(LPCTSTR pszSoundId)
{
   return StartPlay(m_spSoundSource, pszSoundId, false, false);
}

void AudioManager::PlayMusic(LPCTSTR pszMusicId)
{
   StartPlay(m_spMusicSource, pszMusicId, false, false);
}

float AudioManager::GetVolume(T_enVolumeType enVolumeType) const
{
   switch (enVolumeType)
   {
   case enVolumeUserInterface:
      return m_spSoundSource->Gain();
   case enVolumeBackgroundMusic:
      return m_spMusicSource->Gain();
   case enVolumePositional:
      return m_fVolumePositional;
   default:
      ATLASSERT(false);
   }
   return 1.0;
}

void AudioManager::SetVolume(T_enVolumeType enVolumeType, float fValue)
{
   switch (enVolumeType)
   {
   case enVolumeUserInterface:
      m_spSoundSource->Gain(fValue);
      break;
   case enVolumeBackgroundMusic:
      m_spMusicSource->Gain(fValue);
      break;
   case enVolumePositional:
      m_fVolumePositional = fValue;

      std::for_each(m_setPositonalSources.begin(), m_setPositonalSources.end(),
         [&](OpenAL::SourcePtr spSource){
            spSource->Gain(fValue);
         });
      break;
   }
}

void AudioManager::RestartCleanupTimer()
{
   m_timerCleanupBufferMap.expires_from_now(boost::posix_time::seconds(
      c_iBufferMapCleanupElapsedTimeInSeconds));

   m_timerCleanupBufferMap.async_wait(std::bind(&AudioManager::CleanupBufferMap, this, std::placeholders::_1));
}

void AudioManager::CleanupBufferMap(const boost::system::error_code& ec)
{
   if (ec)
      return; // timer was canceled

   m_namedBufferMap.Clean(TimeSpan(0,0,c_iBufferMapCleanupElapsedTimeInSeconds));

   RestartCleanupTimer();
}

std::shared_ptr<IPlaybackControl> AudioManager::StartPlay(OpenAL::SourcePtr spSource, const CString& cszSoundId,
   bool bLooping, bool bFadein)
{
   std::shared_ptr<PlaybackControl> spPlaybackControl(new PlaybackControl(spSource));

   // note: for some reason using std::bind here gives many errors in the functional header at line 1519
   m_ioServiceThread.Get().post(
      boost::bind(&AudioManager::AsyncPlay, this, spSource, cszSoundId, spPlaybackControl, bLooping, bFadein));

   return spPlaybackControl;
}

void AudioManager::AsyncPlay(OpenAL::SourcePtr spSource, const CString& cszSoundId,
                             std::shared_ptr<PlaybackControl> spPlaybackControl,
                             bool bLooping, bool /*bFadein*/)
{
   OpenAL::BufferPtr spBuffer = AudioManager::ResolveBuffer(cszSoundId);

   if (spSource->IsPlaying())
      spSource->Stop();

   spSource->Attach(spBuffer);
   spSource->Looping(bLooping);
   spSource->Play();
}

OpenAL::BufferPtr AudioManager::ResolveBuffer(LPCTSTR pszSoundId)
{
   ATLASSERT(pszSoundId != nullptr);

   OpenAL::BufferPtr spBuffer = m_namedBufferMap.GetBuffer(pszSoundId);
   if (spBuffer != nullptr)
      return spBuffer;

   if (m_fnResolveFileStream == nullptr)
      throw Exception(_T("no stream resolver set"), __FILE__, __LINE__);

   if (m_mapSoundIds.find(pszSoundId) == m_mapSoundIds.end())
      throw Exception(_T("sound id could not be mapped to relative filename"), __FILE__, __LINE__);

   CString cszFilename = m_mapSoundIds[pszSoundId];

   std::shared_ptr<Stream::IStream> spStream = m_fnResolveFileStream(cszFilename);

   spBuffer = ReadOggVorbisFile(spStream);

   m_namedBufferMap.Add(pszSoundId, spBuffer, false);

   return spBuffer;
}

OpenAL::BufferPtr AudioManager::ReadOggVorbisFile(std::shared_ptr<Stream::IStream> spStream) const
{
   using Stream::FileStream;

   Timer t;
   t.Start();

   OggVorbisFileReader reader;
   reader.Read(*spStream);

   if (reader.Channels() != 1 && reader.Channels() != 2)
      throw Exception(_T("Ogg Vorbis: cannot read multichannel files"), __FILE__, __LINE__);

   OpenAL::BufferPtr spBuffer = m_audioDevice.CreateBuffer();

   const std::vector<short>& vecSamples = reader.Samples();

   if (vecSamples.empty())
      throw Exception(_T("Ogg Vorbis: cannot play empty file"), __FILE__, __LINE__);

   if (reader.Channels() == 1)
      spBuffer->SetMono(&vecSamples[0], vecSamples.size(), reader.Samplerate());
   else if (reader.Channels() == 2)
      spBuffer->SetStereo(&vecSamples[0], vecSamples.size(), reader.Samplerate());
   else
      ATLASSERT(false);

   CString cszText;
   cszText.Format(_T("loaded ogg vorbis file in %u ms"), unsigned(t.Elapsed()*1000.0));
   LOG_INFO(cszText, Log::Client::Audio);

   return spBuffer;
}
