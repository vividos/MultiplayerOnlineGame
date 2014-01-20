//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MusicDirector.cpp Music director class
//

// includes
#include "StdAfx.h"
#include "MusicDirector.hpp"
#include "IAudioManager.hpp"
#include "IFileSystem.hpp"
#include "MusicConfigLoader.hpp"
#include <functional>
#include <ulib/stream/TextStreamFilter.hpp>
#include "LogCategories.hpp"

using namespace Audio;

/// minimum break length, in seconds
const unsigned int c_iMinBreakLength = 0;

MusicDirector::MusicDirector(IAudioManager& audioManager, IFileSystem& fileSystem) throw()
:m_audioManager(audioManager),
 m_fileSystem(fileSystem),
 m_timer(audioManager.GetIoService()),
 m_enDangerLevel(dangerLevelNormal)
{
   m_rng.seed(
      static_cast<boost::uint32_t>(
         time(NULL) & std::numeric_limits<boost::uint32_t>::max()));
}

MusicDirector::~MusicDirector() throw()
{
   m_timer.cancel();
}

void MusicDirector::DangerLevel(T_enDangerLevel enDangerLevel)
{
   CString cszText;
   cszText.Format(_T("danger level changed, level=%u"),
      static_cast<unsigned int>(enDangerLevel));
   LOG_INFO(cszText, Log::Client::AudioMusicDirector);

   m_enDangerLevel = enDangerLevel;
}

/// \todo not implemented yet
void MusicDirector::OnGameEvent(T_enGameEventType enGameEventType)
{
   CString cszText;
   cszText.Format(_T("game event occured, event=%u"),
      static_cast<unsigned int>(enGameEventType));
   LOG_INFO(cszText, Log::Client::AudioMusicDirector);

   // TODO implement
}

void MusicDirector::ReadMusicConfig()
{
   CString cszFilename = _T("audio\\music.json");

   std::shared_ptr<Stream::IStream> spStream =
      m_fileSystem.OpenFile(cszFilename, true);

   Stream::TextStreamFilter stream(*spStream,
      Stream::ITextStream::textEncodingAnsi,
      Stream::ITextStream::lineEndingCRLF);

   LOG_INFO(_T("reading MusicDirector config file: ") + cszFilename, Log::Client::AudioMusicDirector);
   CString cszJsonText;
   {
      CString cszLine;
      while(!stream.AtEndOfStream())
      {
         stream.ReadLine(cszLine);
         cszJsonText += cszLine;
         cszJsonText += _T("\n");
      }
   }

   MusicConfigLoader loader(m_mapMusicIdToInfo, m_aDangerLevelMusicIds);
   loader.Load(cszJsonText);
}

void MusicDirector::Start()
{
   StartPlayback();
}

void MusicDirector::StartPlayback()
{
   CString cszId;

   T_enDangerLevel enDangerLevel = m_enDangerLevel;
   ATLASSERT(enDangerLevel < dangerLevelMax);

   std::set<CString>& setIds = m_aDangerLevelMusicIds[enDangerLevel];

   bool bFound = false;
   unsigned int uiTries = 0, uiMaxTries = setIds.size() * 2;

   ATLASSERT(setIds.empty() == false); // list must be filled
   boost::uniform_int<> dist(0, setIds.size()-1);
   boost::variate_generator<boost::mt19937&, boost::uniform_int<>> die(m_rng, dist);

   while (!bFound && uiTries < uiMaxTries)
   {
      // find music id of next piece, dependent on danger level
      unsigned int uiIndex = die();

      std::set<CString>::const_iterator iter = setIds.begin();
      std::advance(iter, uiIndex);
      cszId = *iter;

      uiTries++;

      // check if music was played recently
      const MusicInfo& info = m_mapMusicIdToInfo[cszId];
      DateTime dtNextPlayTime = info.m_dtLastPlayed + TimeSpan(0, 0, m_config.m_uiLowestMusicRepeatTime);
      if (dtNextPlayTime > DateTime::Now())
         continue;

      bFound = true;
   }

   if (!bFound)
   {
      LOG_WARN(_T("didn't find appropriate music, starting another break"), Log::Client::AudioMusicDirector);

      StartBreak();
      return;
   }

   ATLASSERT(m_mapMusicIdToInfo.find(cszId) != m_mapMusicIdToInfo.end());
   MusicInfo& info = m_mapMusicIdToInfo[cszId];

   CString cszText;
   cszText.Format(_T("starting new title, id=%s, len=%u"),
      cszId, static_cast<unsigned int>(info.m_tsDuration.TotalSeconds()));
   LOG_INFO(cszText, Log::Client::AudioMusicDirector);

   info.m_dtLastPlayed = DateTime::Now();

   m_audioManager.AddSoundIdMapping(cszId, _T("audio\\") + info.m_cszFilename);

   m_audioManager.PlayMusic(cszId);

   // restart timer
   m_timer.expires_from_now(boost::posix_time::seconds(
      static_cast<long>(info.m_tsDuration.TotalSeconds())));
   m_timer.async_wait(std::bind(&MusicDirector::OnMusicPlaybackFinished, this, std::placeholders::_1));
}

void MusicDirector::StartBreak()
{
   // generate new break time
   boost::normal_distribution<> dist(
      m_config.m_uiAverageBreakDuration,  // mean value
      m_config.m_uiStandardDeviationDuration); // sigma value

   boost::variate_generator<boost::mt19937&, boost::normal_distribution<>> die(m_rng, dist);
   
   int iBreakLength = static_cast<int>(die());

   unsigned int uiBreakLength = c_iMinBreakLength;
   if (iBreakLength >= int(c_iMinBreakLength))
      uiBreakLength = static_cast<unsigned int>(iBreakLength);

   CString cszText;
   cszText.Format(_T("starting break, len=%i"),
      iBreakLength);
   LOG_INFO(cszText, Log::Client::AudioMusicDirector);

   // restart timer
   m_timer.expires_from_now(boost::posix_time::seconds(uiBreakLength));
   m_timer.async_wait(std::bind(&MusicDirector::OnMusicBreakFinished, this, std::placeholders::_1));
}

void MusicDirector::OnMusicPlaybackFinished(const boost::system::error_code& ec)
{
   if (ec)
      return; // break was canceled

   StartBreak();
}

void MusicDirector::OnMusicBreakFinished(const boost::system::error_code& ec)
{
   if (ec)
      return; // break was canceled

   StartPlayback();
}
