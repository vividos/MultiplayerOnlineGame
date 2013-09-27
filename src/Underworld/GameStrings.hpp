//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/GameStrings.hpp Game strings
//
#pragma once

// includes
#include <map>
#include <vector>
#include <string>
#include <set>

// forward references
namespace Import
{
class StringsPakFile;
}

// classes

/// \brief game strings class
/// Game strings are contained in blocks that contain a list of strings. Each
/// block contains specific strings, for item descriptions, cutscene text or
/// conversations.
class GameStrings
{
public:
   /// ctor
   GameStrings() {}

   /// returns if block id is available
   bool IsBlockAvail(Uint16 uiBlockId) const;

   /// returns a whole string block
   const std::vector<std::string>& GetStringBlock(Uint16 uiBlockId);

   /// returns a string from given block
   std::string Get(Uint16 uiBlockId, unsigned int uiStringNr);

private:
   friend class Import::StringsPakFile;

   /// a map with all string blocks
   std::map<Uint16, std::vector<std::string>> m_mapAllStrings;
};
