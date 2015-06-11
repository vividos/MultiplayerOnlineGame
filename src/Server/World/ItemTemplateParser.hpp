//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ItemTemplateParser.hpp Item template parser
//
#pragma once

// includes
#include "World.hpp"
#include "ItemTemplate.hpp"
#include "StringTokenizer.hpp"

/// parses item description and produces ItemTemplate object
class WORLD_DECLSPEC ItemTemplateParser
{
public:
   /// ctor
   ItemTemplateParser() throw() {}

   /// parses item description
   std::shared_ptr<ItemTemplate> Parse(LPCTSTR pszText);

private:
   /// spell string tokenizer
   StringTokenizer m_tokenizer;
};
