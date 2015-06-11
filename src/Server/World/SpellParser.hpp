//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SpellParser.hpp Spell parser
//
#pragma once

// includes
#include "World.hpp"
#include "SpellEffect.hpp"
#include "StringTokenizer.hpp"

// forward references
class Spell;

/// parser for spell text
class WORLD_DECLSPEC SpellParser
{
public:
   /// ctor
   SpellParser() throw() {}

   /// parses spell description
   std::shared_ptr<Spell> Parse(LPCTSTR pszText);

private:
   /// spell string tokenizer
   StringTokenizer m_tokenizer;

private:
   /// parses single spell effect
   std::shared_ptr<SpellEffect> ParseEffect();

   /// parses instant / over time effect attributes
   void ParseInstantOverTime(std::shared_ptr<DamageHealSpellEffectBase> spDamageOrHealEffect);

   /// parses unsigned int value
   static unsigned int ParseUint(const CString& cszText);

   /// parses signed int value
   static int ParseInt(const CString& cszText);

   /// parses time value; may have suffix s, m, h or d
   static unsigned int TimeToSeconds(const CString& cszText);

   /// parses value or range
   static EffectValueOrRange ParseValueOrRange(const CString& cszText);
};
