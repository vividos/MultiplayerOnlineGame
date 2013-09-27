//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/ConvManager.cpp Conversation manager
//

// includes
#include "stdafx.h"
#include "ConvManager.hpp"
#include "Underworld.hpp"
#include "IFileSystem.hpp"
#include "ConvLoader.hpp"
#include "GameStrings.hpp"

void ConvManager::Load(IFileSystem& fileSystem, Uint8 npc_whoami)
{
   Import::ConvLoader::LoadConvCode(fileSystem, m_conv, npc_whoami);

   m_uiStringBlock = m_conv.StringBlock();

   m_npc = m_strings.Get(7, m_uiStringBlock - 0x0e00 + 16);
}

void ConvManager::Run(unsigned int uwLevel, Uint16 uiNpcPos)
{
   m_conv.Init(uwLevel, uiNpcPos, this, m_vecLocalStrings);

   while (m_conv.Step());

   m_conv.Done();
}

void ConvManager::Say(Uint16 index)
{
   std::string text = m_strings.Get(m_uiStringBlock, index);
   m_conv.ReplacePlaceholder(text);

   ATLTRACE(_T("%hs says: %hs\n"), m_npc.c_str(), text.c_str());
}

Uint16 ConvManager::BablMenu(const std::vector<Uint16>& uiAnswerStringIds)
{
   ATLTRACE(_T("You say:\n"));

   for (size_t i=0, iMax = uiAnswerStringIds.size(); i<iMax; i++)
   {
      std::string text = m_strings.Get(m_uiStringBlock, uiAnswerStringIds[i]);
      m_conv.ReplacePlaceholder(text);

      ATLTRACE(_T("%u> %hs\n"), i+1, text.c_str());
   }

   return 1; // for now
}

Uint16 ConvManager::ExternalFunc(const char* pszFuncName, Conv::ConvStack& /*stack*/)
{
   ATLTRACE(_T("ExternalFunc = %hs\n"), pszFuncName);
   return 0;
}
