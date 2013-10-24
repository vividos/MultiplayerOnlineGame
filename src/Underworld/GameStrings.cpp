//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/GameStrings.cpp Game strings
//

// includes
#include "stdafx.h"
#include "GameStrings.hpp"

// GameStrings methods

bool GameStrings::IsBlockAvail(Uint16 uiBlockId) const
{
   return m_mapAllStrings.find(uiBlockId) != m_mapAllStrings.end();
}

const std::vector<std::string>& GameStrings::GetStringBlock(Uint16 uiBlockId)
{
   if (!IsBlockAvail(uiBlockId))
   {
      UaTrace("string block %04x cannot be found\n", uiBlockId);

      static std::vector<std::string> s_vecEmptyBlock;
      return s_vecEmptyBlock;
   }

   return m_mapAllStrings[uiBlockId];
}

std::string GameStrings::Get(Uint16 uiBlockId, unsigned int uiStringNr)
{
   std::string text;
   if (!IsBlockAvail(uiBlockId))
   {
      UaTrace("string block %04x cannot be found\n", uiBlockId);
      return text;
   }

   const std::vector<std::string>& vecStringBlock = m_mapAllStrings[uiBlockId];
   if (uiStringNr < vecStringBlock.size())
      text = vecStringBlock[uiStringNr];

   return text;
}
