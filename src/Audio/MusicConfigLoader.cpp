//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MusicConfigLoader.cpp Music config file loader
//

// includes
#include "StdAfx.h"
#include "MusicConfigLoader.hpp"
#include <boost/foreach.hpp>
#include <json/json.h>
#pragma comment(lib, "jsoncpp.lib")
#include "StringTools.hpp"

using Audio::MusicConfigLoader;

LPCSTR c_pszaId = "id";
LPCSTR c_pszaLen = "len";
LPCSTR c_pszaFile = "file";
LPCSTR c_pszaTags = "tags";

LPCTSTR c_pszDangerLevelSafe = _T("safe");
LPCTSTR c_pszDangerLevelNormal = _T("normal");
LPCTSTR c_pszDangerLevelDungeon = _T("dungeon");

void MusicConfigLoader::Load(const CString& cszJsonText)
{
   Json::Reader reader;

   // convert text to UTF-8
   std::vector<unsigned char> vecUtf8Text;
   StringTools::StringToUTF8(cszJsonText, vecUtf8Text);

   std::string strUtf8JsonText(reinterpret_cast<const char*>(&vecUtf8Text[0]), vecUtf8Text.size());

   // parse
   Json::Value root;
   if (!reader.parse(strUtf8JsonText, root, false))
   {
      CString cszText;
      cszText.Format(_T("error while parsing json data: %hs"),
         reader.getFormatedErrorMessages().c_str());
      throw Exception(cszText, __FILE__, __LINE__);
   }

   WalkTrackArray(root);
}

void MusicConfigLoader::WalkTrackArray(Json::Value& node)
{
   ATLASSERT(node.isArray());

   // enumerate array member
   for (Json::Value::UInt index = 0, max = node.size(); index < max; index++)
   {
      Json::Value valItem = node[index];
      ReadTrackObject(valItem);
   }
}

void MusicConfigLoader::ReadTrackObject(Json::Value& node)
{
   ATLASSERT(node.isObject());

   // extract object member
   ATLASSERT(node.isMember(c_pszaId));
   ATLASSERT(node.isMember(c_pszaLen));
   ATLASSERT(node.isMember(c_pszaFile));
   ATLASSERT(node.isMember(c_pszaTags));

   CString cszId = node.get(c_pszaId, Json::Value::null).asCString();
   unsigned int uiLen = node.get(c_pszaLen, Json::Value::null).asUInt();
   CString cszFile = node.get(c_pszaFile, Json::Value::null).asCString();
   Json::Value tags = node.get(c_pszaTags, Json::Value::null);

   std::set<CString> setTags;
   ReadTags(tags, setTags);

   AddMusicTrack(cszId, uiLen, cszFile, setTags);
}

void MusicConfigLoader::ReadTags(Json::Value& node, std::set<CString>& setTags)
{
   ATLASSERT(node.isArray());

   // enumerate array member
   for (Json::Value::UInt index = 0, max = node.size(); index < max; index++)
   {
      Json::Value valItem = node[index];
      setTags.insert(CString(valItem.asCString()));
   }
}

void MusicConfigLoader::AddMusicTrack(const CString& cszId, unsigned int uiLen, const CString& cszFilename,
                                      std::set<CString>& setTags)
{
   ATLTRACE(_T("music track: id=%s, len=%u, file=%s, %u tags\n"),
      cszId, uiLen, cszFilename, setTags.size());

   // add music info
   MusicDirector::MusicInfo musicInfo;
   musicInfo.m_cszFilename = cszFilename;
   musicInfo.m_dtLastPlayed = DateTime::MinValue();
   musicInfo.m_tsDuration = TimeSpan(0, 0, uiLen);

   m_mapMusicIdToInfo.insert(std::make_pair(cszId, musicInfo));

   // add id to all appropriate danger levels
   BOOST_FOREACH(const CString& cszTag, setTags)
   {
      if (cszTag == c_pszDangerLevelSafe)    m_aDangerLevelMusicIds[MusicDirector::dangerLevelSafe].insert(cszId);
      if (cszTag == c_pszDangerLevelNormal)  m_aDangerLevelMusicIds[MusicDirector::dangerLevelNormal].insert(cszId);
      if (cszTag == c_pszDangerLevelDungeon) m_aDangerLevelMusicIds[MusicDirector::dangerLevelDungeon].insert(cszId);
   }
}
