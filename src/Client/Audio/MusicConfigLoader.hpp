//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MusicConfigLoader.hpp Music config file loader
//
#pragma once

// includes
#include "MusicDirector.hpp"
#include <set>
#include <map>

// forward references
namespace Json
{
   class Value;
}

namespace Audio
{

/// background music config loader
class MusicConfigLoader
{
public:
   /// ctor
   MusicConfigLoader(
      std::map<CString, MusicDirector::MusicInfo>& mapMusicIdToInfo,
      MusicDirector::T_aDangerLevelMusicIds& aDangerLevelMusicIds)
      :m_mapMusicIdToInfo(mapMusicIdToInfo),
       m_aDangerLevelMusicIds(aDangerLevelMusicIds)
   {
   }

   /// loads config from json string
   void Load(const CString& cszJsonText);

private:
   /// processes track array
   void WalkTrackArray(Json::Value& node);

   /// reads single track object
   void ReadTrackObject(Json::Value& node);

   /// reads all tags
   void ReadTags(Json::Value& node, std::set<CString>& setTags);

   /// adds music track
   void AddMusicTrack(const CString& cszId, unsigned int uiLen, const CString& cszFilename,
      std::set<CString>& setTags);

private:
   /// map type for music ids to music info
   std::map<CString, MusicDirector::MusicInfo>& m_mapMusicIdToInfo;

   /// music ids for all danger levels
   MusicDirector::T_aDangerLevelMusicIds& m_aDangerLevelMusicIds;
};

} // namespace Audio
