//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Import/ObjectListLoader.hpp Object list loader
//
#pragma once

// includes
#include "ObjectList.hpp"

namespace Import
{

/// loads object list
class ObjectListLoader
{
public:
   /// ctor
   ObjectListLoader(Underworld::ObjectList& objectList,
      std::vector<Uint16>& vecObjectWords,
      std::vector<Uint8>& vecNpcInfos,
      std::vector<Uint16>& vecTextureMapping)
      :m_objectList(objectList),
      m_vecObjectWords(vecObjectWords),
      m_vecNpcInfos(vecNpcInfos),
      m_vecTextureMapping(vecTextureMapping)
   {}

   /// follows link and adds all objects it finds (may get called recursively)
   void FollowLink(Uint16 uiLink, Uint8 xpos, Uint8 ypos);

   /// adds object to object list
   Underworld::ObjectPtr AddObject(Uint16 uiPos, Uint8 xpos, Uint8 ypos,
      Uint16 uiObjectWord[4], Uint8* pNpcInfos);

   /// adds NPC infos to object
   void AddNpcInfos(Underworld::ObjectPtr& obj, Uint8* pNpcInfos);

private:
   /// object list
   Underworld::ObjectList& m_objectList;

   /// object list words (4x Uint16 for each slot)
   std::vector<Uint16>& m_vecObjectWords;

   /// NPC info bytes (19x Uint8 for each NPC)
   std::vector<Uint8>& m_vecNpcInfos;

   /// texture mapping
   std::vector<Uint16>& m_vecTextureMapping;
};

} // namespace Import
