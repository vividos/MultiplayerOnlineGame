//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ItemTemplateParser.cpp Item template parser
//

// includes
#include "StdAfx.h"
#include "ItemTemplateParser.hpp"

std::shared_ptr<ItemTemplate> ItemTemplateParser::Parse(LPCTSTR pszText)
{
   m_tokenizer = StringTokenizer(pszText);

   // TODO
   return std::shared_ptr<ItemTemplate>();
}
