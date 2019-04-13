//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MusicDirector.hpp Music director class
//
#pragma once

// includes
#include "Audio.hpp"
#include <random>
#include <array>
#include <ulib/DateTime.hpp>
#include <map>
#include <set>

// forward references
class IFileSystem;

namespace Audio
{
class IAudioManager;

/// configuration for the MusicDirector class
class MusicDirectorConfig
{
public:
   /// ctor
   MusicDirectorConfig()
      :m_uiAverageBreakDuration(2*60), // 2 minutes
       m_uiStandardDeviationDuration(30), // 30 seconds
       m_uiLowestMusicRepeatTime(30*60) // 30 minutes
   {
   }

   /// average break duration, in seconds
   unsigned int m_uiAverageBreakDuration;

   /// standard deviation sigma of duration, in seconds; 95% of breaks are +/- 2*sigma
   unsigned int m_uiStandardDeviationDuration;

   /// lowest number of seconds after a song may be repeated, in seconds
   unsigned int m_uiLowestMusicRepeatTime;
};

/// \brief background music director
/// \details manages playback of background music, based on available music
/// pieces, danger level and ingame events. The music director plays a music
/// piece with a break of varying length. Played back pieces are chosen based
/// on how dangerous the current area is. In a break, game events can trigger
/// other pieces, prematurely ending the break.
class AUDIO_DECLSPEC MusicDirector
{
public:
   /// ctor
   MusicDirector(IAudioManager& audioManager, IFileSystem& fileSystem);
   /// dtor
   ~MusicDirector();

   /// returns configuration
   MusicDirectorConfig& Config() { return m_config; }

   /// reads music config file
   void ReadMusicConfig();

   /// starts music director
   void Start();

   /// danger level
   enum T_enDangerLevel
   {
      dangerLevelSafe = 0,
      dangerLevelNormal,
      dangerLevelDungeon,

      dangerLevelMax
   };

   /// sets new danger level
   void DangerLevel(T_enDangerLevel enDangerLevel);

   /// game event type
   enum T_enGameEventType
   {
      gameEventEnterNewArea = 0,
      gameEventCombatEnter,
      gameEventCombatFinished,
   };

   /// called when game event occurs
   void OnGameEvent(T_enGameEventType enGameEventType);

private:
   /// start a new music piece
   void StartPlayback();

   /// start a new break
   void StartBreak();

   /// called by timer when music playback has finished
   void OnMusicPlaybackFinished(const boost::system::error_code& ec);

   /// called by timer when break has finished
   void OnMusicBreakFinished(const boost::system::error_code& ec);

private:
   friend class MusicConfigLoader;

   /// audio manager to use for playback
   IAudioManager& m_audioManager;

   /// file system to use for loading music and playlist
   IFileSystem& m_fileSystem;

   /// configuration
   MusicDirectorConfig m_config;

   /// RNG
   std::mt19937 m_rng;

   /// current danger level
   T_enDangerLevel m_enDangerLevel;

   /// id of music currently playing back
   CString m_cszCurrentMusicId;

   /// last playback start
   DateTime m_dtPlaybackStart;

   /// timer
   boost::asio::deadline_timer m_timer;

   /// type for music ids for every danger level
   typedef std::array<std::set<CString>, dangerLevelMax> T_aDangerLevelMusicIds;

   /// music ids for every danger level
   T_aDangerLevelMusicIds m_aDangerLevelMusicIds;

   /// music info
   struct MusicInfo
   {
      CString m_cszFilename;     ///< music filename
      DateTime m_dtLastPlayed;   ///< time of music last played
      TimeSpan m_tsDuration;     ///< duration of music
   };

   /// mapping from music id to music info
   std::map<CString, MusicInfo> m_mapMusicIdToInfo;
};

} // namespace Audio
