//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/ConvManager.hpp Conversation manager
//

// includes
#include "stdafx.h"
#include "Underworld.hpp"
#include "Conversation.hpp"

// forward references
class IFileSystem;
class GameStrings;

namespace Underworld
{
class World;
}

/// conversation manager
class ConvManager: public Conv::CodeCallback
{
public:
   /// ctor
   ConvManager(Underworld::World& world, GameStrings& strings)
      :m_strings(strings),
       m_conv(world),
       m_uiStringBlock(0x0e00)
   {
   }
   /// dtor
   virtual ~ConvManager() {}

   /// Loads conversation
   void Load(IFileSystem& fileSystem, Uint8 npc_whoami);

   /// runs conversation for npc on given level
   void Run(unsigned int uwLevel, Uint16 uiNpcPos);

private:
   // virtual methods from Conv::CodeCallback

   /// prints "say" string
   virtual void Say(Uint16 index) override;

   /// shows menu, with possible answers
   virtual Uint16 BablMenu(const std::vector<Uint16>& uiAnswerStringIds) override;

   /// executes external function
   virtual Uint16 ExternalFunc(const char* pszFuncName, Conv::ConvStack& /*stack*/) override;

private:
   /// game strings
   GameStrings& m_strings;

   /// conversation
   Conv::Conversation m_conv;

   /// string block to use
   Uint16 m_uiStringBlock;

   /// NPC name
   std::string m_npc;

   /// local strings
   std::vector<std::string> m_vecLocalStrings;
};
