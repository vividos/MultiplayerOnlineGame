//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file SpellParser.cpp Spell parser
//

// includes
#include "StdAfx.h"
#include "SpellParser.hpp"
#include "Spell.hpp"

std::shared_ptr<Spell> SpellParser::Parse(LPCTSTR pszText)
{
   m_tokenizer = StringTokenizer(pszText);

   unsigned int uiSpellId = ParseUint(m_tokenizer.Next());
   unsigned int uiCooldown = 0;
   unsigned int uiCastTime = 0;
   bool bStackable = false;
   unsigned int uiRange = 0;
   bool bArea = false;
   std::vector<std::shared_ptr<SpellEffect>> vecEffects;

   CString cszToken;
   while (!m_tokenizer.IsEmpty())
   {
      cszToken = m_tokenizer.Next();

      if (cszToken == _T("cooldown"))
      {
         uiCooldown = TimeToSeconds(m_tokenizer.Next());
      }
      else
      if (cszToken == _T("cast-time"))
      {
         uiCastTime = TimeToSeconds(m_tokenizer.Next());
      }
      else
      if (cszToken == _T("stackable"))
      {
         bStackable = true;
      }
      else
      if (cszToken == _T("range"))
      {
         uiRange = ParseUint(m_tokenizer.Next());
      }
      else
      if (cszToken == _T("area"))
      {
         bArea = true;
      }
      else
      if (cszToken == _T("effect"))
      {
         ATLVERIFY(_T("[") == m_tokenizer.Next());
         vecEffects.push_back(ParseEffect());
      }
   }

   if (vecEffects.empty())
      throw Exception(_T("error parsing spell: no effect specified"), __FILE__, __LINE__);

   std::shared_ptr<Spell> spSpell(new Spell(uiSpellId, vecEffects[0]));

   spSpell->Cooldown(uiCooldown);
   spSpell->CastTime(uiCastTime);
   spSpell->Stackable(bStackable);
   spSpell->Range(uiRange);
   spSpell->AreaSpell(bArea);

   if (vecEffects.size() > 2)
      throw Exception(_T("error parsing spell: too many effects specified"), __FILE__, __LINE__);

   if (vecEffects.size() == 2)
      spSpell->Effect2(vecEffects[1]);

   return spSpell;
}

std::shared_ptr<SpellEffect> SpellParser::ParseEffect()
{
   std::shared_ptr<SpellEffect> spEffect;

   // first one is effect type
   CString cszToken = m_tokenizer.Next();
   if (cszToken == _T("damage"))
   {
      std::shared_ptr<DamageSpellEffect> spDamageEffect(new DamageSpellEffect);

      // damage value/range
      spDamageEffect->ValueOrRange(ParseValueOrRange(m_tokenizer.Next()));

      // damage type
      spDamageEffect->SetDamageType(DamageType::FromString(m_tokenizer.Next()));

      ParseInstantOverTime(spDamageEffect);

      // parse restrictions

      spEffect = spDamageEffect;
   }
   else
   if (cszToken == _T("heal"))
   {
      std::shared_ptr<HealSpellEffect> spHealEffect;

      // heal value/range
      spHealEffect->ValueOrRange(ParseValueOrRange(m_tokenizer.Next()));

      ParseInstantOverTime(spHealEffect);

      spEffect = spHealEffect;
   }
   else
   if (cszToken == _T("modify"))
   {
      // TODO impl.
   }
   else
   if (cszToken == _T("resist"))
   {
      // TODO impl.
   }
   else
   if (cszToken == _T("disable"))
   {
      std::shared_ptr<DisableSpellEffect> spDisableEffect(new DisableSpellEffect);

      spDisableEffect->DisableType(DisableSpellEffect::DisableTypeFromString(m_tokenizer.Next()));

      spEffect = spDisableEffect;
   }
   else
      throw Exception(_T("invalid spell effect type"), __FILE__, __LINE__);

   return spEffect;
}

void SpellParser::ParseInstantOverTime(std::shared_ptr<DamageHealSpellEffectBase> spDamageOrHealEffect)
{
   CString cszToken = m_tokenizer.Next();

   if (cszToken == _T("instant"))
   {
      spDamageOrHealEffect->Instant(true);
   }
   else
   if (cszToken == _T("over-time"))
   {
      spDamageOrHealEffect->Instant(false);
      spDamageOrHealEffect->Duration(TimeToSeconds(m_tokenizer.Next()));

      cszToken = m_tokenizer.Next();
      if (cszToken != _T("tick-time"))
         throw Exception(_T("tick-time must follow over-time in spell description"), __FILE__, __LINE__);

      spDamageOrHealEffect->TickTime(TimeToSeconds(m_tokenizer.Next()));
   }
   else
      throw Exception(_T("only instant or over-time are allowed for damage spells"), __FILE__, __LINE__);
}

unsigned int SpellParser::ParseUint(const CString& cszText)
{
   return static_cast<unsigned int>(_ttoi(cszText));
}

int SpellParser::ParseInt(const CString& cszText)
{
   return _ttoi(cszText);
}

unsigned int SpellParser::TimeToSeconds(const CString& cszText)
{
   unsigned int uiMod = 1;
   CString cszSuffix = cszText.Right(1);
   if (cszSuffix == _T("s"))
      uiMod = 1;
   else if (cszSuffix == _T("m"))
      uiMod = 60;
   else if (cszSuffix == _T("h"))
      uiMod = 60*60;
   else if (cszSuffix == _T("d"))
      uiMod = 24*60*60;

   unsigned int uiValue = ParseUint(cszText);
   uiValue *= uiMod;

   return uiValue;
}

EffectValueOrRange SpellParser::ParseValueOrRange(const CString& cszText)
{
   EffectValueOrRange valueOrRange;

   int iPosDash = cszText.Find(_T('-'));
   int iPosPercent = cszText.Find(_T('%'));
   if (iPosDash > 0) // at 0 it is the sign value
   {
      valueOrRange.SetRange(iPosPercent != -1,
         ParseInt(cszText.Left(iPosDash)),
         ParseInt(cszText.Mid(iPosDash+1)));
   }
   else
   {
      valueOrRange.SetValue(iPosPercent != -1,
         ParseInt(cszText));
   }

   return valueOrRange;
}
