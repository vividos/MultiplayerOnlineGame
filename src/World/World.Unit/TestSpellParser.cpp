//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file TestSpellParser.cpp Unit tests for class SpellParser
//

// includes
#include "stdafx.h"
#include "Spell.hpp"
#include "SpellEffect.hpp"
#include "SpellParser.hpp"
#include "ByteStream.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

LPCTSTR apszSpells[] =
{
   _T("4 ")
   _T("effect [damage 500 normal instant]"),

   _T("4 cooldown 5h ")
   _T("effect [damage 200-300 normal instant]"),

   _T("4 cast-time 2s ")
   _T("effect [damage 150% normal instant]"),

   _T("4 area range 10 ")
   _T("effect [damage 20%-30% normal instant]"),

   _T("4 stackable ")
   _T("effect [damage 500 normal instant]"),

   _T("4 ")
   _T("effect [damage 500 shadow over-time 2m tick-time 20s]"),

   _T("4 cooldown 10s cast-time 2s stackable area range 10 ")
   _T("effect [damage 500 normal over-time 10s tick-time 2s]"),

   _T("4 stackable ")
   _T("effect [damage 2580-2630 fire instant] ")
   _T("effect [damage 500 frost over-time 10s tick-time 2s]"),

   _T("4 ")
   _T("effect [damage 500 normal over-time 10s tick-time 2s] ")
   _T("effect [disable spellcast]"),
};

/// tests class SpellParser
TEST_CLASS(TestSpellParser)
{
   /// tests spell parser
   TEST_METHOD(TestParser)
   {

      for (size_t i=0, iMax=sizeof(apszSpells)/sizeof(*apszSpells); i<iMax; i++)
      {
         // test parsing
         ATLTRACE(_T("spell: %s"), apszSpells[i]);
         SpellParser parser;
         std::shared_ptr<Spell> spSpell = parser.Parse(apszSpells[i]);

         ATLTRACE(_T("parsed: %s"), spSpell->ToString());

         // test serializing
         VectorStream s;
         spSpell->Serialize(s);

         // try to deserialize
         std::shared_ptr<Spell> spSpell2 = Spell::Construct(s);

         ATLTRACE(_T("deserialized: %s"), spSpell2->ToString());

         Assert::AreEqual(spSpell->ToString().GetString(), spSpell2->ToString().GetString());

         ATLTRACE(_T(""));
      }
   }
};

} // namespace UnitTest
