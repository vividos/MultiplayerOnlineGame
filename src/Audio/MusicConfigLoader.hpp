//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file MusicConfigLoader.hpp Music config file loader
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
   void WalkTrackArray(Json::Value& node);
   void ReadTrackObject(Json::Value& node);
   void ReadTags(Json::Value& node, std::set<CString>& setTags);

   void AddMusicTrack(const CString& cszId, unsigned int uiLen, const CString& cszFilename,
      std::set<CString>& setTags);

private:
   std::map<CString, MusicDirector::MusicInfo>& m_mapMusicIdToInfo;
   MusicDirector::T_aDangerLevelMusicIds& m_aDangerLevelMusicIds;
};

} // namespace Audio
