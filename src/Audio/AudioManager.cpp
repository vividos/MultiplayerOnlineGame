//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file AudioManager.hpp Audio manager implementation
//

// includes
#include "StdAfx.h"
#include "AudioManager.hpp"
#include "Source.hpp"
#include "LogCategories.hpp"
#include "Timer.hpp"
#include "OggVorbisFileReader.hpp"
#include <ulib/stream/FileStream.hpp>
#include "Filesystem.hpp"

using Audio::IAudioManager;
using Audio::AudioManager;

IMPLEMENT_SINGLETON(AudioManager)

//
// IAudioManager
//

IAudioManager& IAudioManager::Get()
{
   return AudioManager::Instance();
}


//
// AudioManager
//

AudioManager::AudioManager()
:m_ioServiceThread(true, _T("Audio Subsystem Thread")) // has default work
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

   m_spMusicSource = boost::dynamic_pointer_cast<Source>(CreateSource());
}

AudioManager::~AudioManager() throw()
{
   m_ioServiceThread.Get().stop();
   m_ioServiceThread.Join();

   LOG_INFO(_T("stopped audio subsystem"), Log::Client::Audio);
}

void AudioManager::PlayMusic(LPCTSTR pszMusicId, LPCTSTR pszFilename)
{
   m_ioServiceThread.Get().post(
      boost::bind(&AudioManager::AsyncPlayMusic, this, CString(pszMusicId), CString(pszFilename)));
}

void AudioManager::AsyncPlayMusic(const CString& cszMusicId, const CString& cszFilename)
{
   OpenAL::BufferPtr spBuffer = m_namedBufferMap.GetBuffer(cszMusicId);
   if (spBuffer == NULL)
   {
      // not loaded yet
      spBuffer = ReadOggVorbisFile(cszFilename);
   }

   m_namedBufferMap.Add(cszMusicId, spBuffer);

   m_spMusicSource->Play(spBuffer, false, false);
}

void AudioManager::AsyncPlay(boost::shared_ptr<Audio::Source> spSource, LPCTSTR pszSoundId, bool bLoop, bool bFadeIn)
{
   m_ioServiceThread.Get().post(
      boost::bind(&AudioManager::LoadAndPlay, this, spSource, pszSoundId, bLoop, bFadeIn));
}

void AudioManager::LoadAndPlay(boost::shared_ptr<Audio::Source> spSource, LPCTSTR pszSoundId, bool bLoop, bool bFadeIn)
{
   // TODO resolve pszSoundId to file name
   //CString cszFilename = Filesystem().BaseFolder() + _T("audio\\town-in-ruins-loop-mono.ogg");
   CString cszFilename = Filesystem().BaseFolder() + _T("audio\\click2-cut.ogg");

   OpenAL::BufferPtr spBuffer = ReadOggVorbisFile(cszFilename);

   m_namedBufferMap.Add(pszSoundId, spBuffer);

   spSource->Play(spBuffer, bLoop, bFadeIn);
}

OpenAL::BufferPtr AudioManager::ReadOggVorbisFile(const CString& cszFilename) const
{
   using Stream::FileStream;

   Timer t;
   t.Start();

   FileStream fs(cszFilename,
      FileStream::modeOpen, FileStream::accessRead, FileStream::shareRead);

   OggVorbisFileReader reader;
   reader.Read(fs);

   // TODO replace throw with log?
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
   cszText.Format(_T("loaded ogg vorbis file in %u ms: [%s]"),
      unsigned(t.Elapsed()*1000.0), cszFilename);
   LOG_INFO(cszText, Log::Client::Audio);

   return spBuffer;
}
