//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SpellEffect.hpp Spell effect
//

// includes
#include "StdAfx.h"
#include "SpellEffect.hpp"
#include "ByteStream.hpp"
#include "Flags8.hpp"

// EffectValueOrRange methods

void EffectValueOrRange::Serialize(ByteStream& stream) const
{
   Flags8 flags;
   flags.SetBit<0>(m_bRange);
   flags.SetBit<1>(m_bPercent);

   stream.Write8(flags.Value());

   stream.Write32(static_cast<unsigned int>(m_iValues[0]));
   if (m_bRange)
      stream.Write32(static_cast<unsigned int>(m_iValues[1]));
}

void EffectValueOrRange::Deserialize(ByteStream& stream)
{
   // read flag
   Flags8 flags = stream.Read8();

   m_bRange = flags.GetBit<0>();
   m_bPercent = flags.GetBit<1>();

   m_iValues[0] = static_cast<int>(stream.Read32());
   if (m_bRange)
      m_iValues[1] = static_cast<int>(stream.Read32());
}

CString EffectValueOrRange::ToString() const throw()
{
   CString cszText;

   cszText.Format(IsRange() ? _T("%i%s-%i%s") : _T("%i%s"),
      m_iValues[0], m_bPercent ? _T("%") : _T(""),
      m_iValues[1], m_bPercent ? _T("%") : _T(""));

   return cszText;
}


// SpellEffect methods

std::shared_ptr<SpellEffect> SpellEffect::Construct(ByteStream& stream)
{
   // read type
   unsigned char ucType = stream.Read8();
   if (ucType >= typeMax)
      throw Exception(_T("invalid spell effect type"), __FILE__, __LINE__);

   T_enEffectType enEffectType = static_cast<T_enEffectType>(ucType);

   static std::shared_ptr<SpellEffect> spEffect;
   switch (enEffectType)
   {
   case SpellEffect::typeDamage: spEffect.reset(new DamageSpellEffect); break;
   case SpellEffect::typeHeal: spEffect.reset(new HealSpellEffect); break;
   case SpellEffect::typeModify: spEffect.reset(new ModifyAttrSpellEffect); break;
   case SpellEffect::typeResist: spEffect.reset(new ResistSpellEffect); break;
   case SpellEffect::typeDisable: spEffect.reset(new DisableSpellEffect); break;
   default: ATLASSERT(false); break;
   }

   ATLASSERT(spEffect != NULL);
   spEffect->Deserialize(stream);

   return spEffect;
}

void SpellEffect::Serialize(ByteStream& stream) const
{
   stream.Write8(static_cast<unsigned char>(m_enEffectType));
   m_valueOrRange.Serialize(stream);
}

void SpellEffect::Deserialize(ByteStream& stream)
{
   // omit reading m_enEffectType; already done in Construct()
   m_valueOrRange.Deserialize(stream);
}


// DamageHealSpellEffectBase methods

void DamageHealSpellEffectBase::Serialize(ByteStream& stream) const
{
   BaseClass::Serialize(stream);

   Flags8 flags;
   flags.SetBit<0>(m_bIsInstant);
   flags.SetBit<1>(m_uiDuration != 0);
   flags.SetBit<2>(m_uiTickTime != 0);

   stream.Write8(flags.Value());

   if (m_uiDuration != 0) stream.Write32(m_uiDuration);
   if (m_uiTickTime != 0) stream.Write32(m_uiTickTime);
}

void DamageHealSpellEffectBase::Deserialize(ByteStream& stream)
{
   BaseClass::Deserialize(stream);

   // read flag
   Flags8 flags = stream.Read8();

   m_bIsInstant = flags.GetBit<0>();

   bool bHasDuration = flags.GetBit<1>();
   bool bHasTickTime = flags.GetBit<2>();

   m_uiDuration = bHasDuration ? stream.Read32() : 0;
   m_uiTickTime = bHasTickTime ? stream.Read32() : 0;
}


// DamageSpellEffect methods

bool DamageSpellEffect::Restriction(T_enDamageEffectRestriction enRestriction) const
{
   Flags8 flags = m_ucRestriction;
   switch (enRestriction)
   {
   case noArmor: return flags.GetBit<0>();
   case noParryBlock: return flags.GetBit<1>();
   }
   throw Exception(_T("invalid damage effect restriction"), __FILE__, __LINE__);
}

void DamageSpellEffect::Restriction(T_enDamageEffectRestriction enRestriction, bool bRestrict)
{
   Flags8 flags = m_ucRestriction;
   switch (enRestriction)
   {
   case noArmor: flags.SetBit<0>(bRestrict);
   case noParryBlock: flags.SetBit<1>(bRestrict);
   default:
      throw Exception(_T("invalid damage effect restriction"), __FILE__, __LINE__);
   }

   m_ucRestriction = flags.Value();
}

void DamageSpellEffect::Serialize(ByteStream& stream) const
{
   BaseClass::Serialize(stream);

   stream.Write8(static_cast<unsigned char>(m_damageType.Type()));
   stream.Write8(m_ucRestriction);
}

void DamageSpellEffect::Deserialize(ByteStream& stream)
{
   BaseClass::Deserialize(stream);

   unsigned char ucDamageType = stream.Read8();
   m_damageType = DamageType(static_cast<DamageType::T_enDamageType>(ucDamageType));
   if (m_damageType.Type() >= DamageType::typeMax)
      throw Exception(_T("invalid damage type"), __FILE__, __LINE__);

   m_ucRestriction = stream.Read8();
   if (m_ucRestriction > 3)
      throw Exception(_T("invalid damage restriction flags"), __FILE__, __LINE__);
}

CString DamageSpellEffect::ToString() const throw()
{
   CString cszText;

   cszText.Format(IsInstant() ? _T("damage %s instant") : _T("damage %s over-time %us tick-time %us"),
      m_damageType.ToString(),
      m_uiDuration, m_uiTickTime);
   // TODO add restrictions

   return cszText;
}


// HealSpellEffect methods

void HealSpellEffect::Serialize(ByteStream& stream) const
{
   BaseClass::Serialize(stream);
}

void HealSpellEffect::Deserialize(ByteStream& stream)
{
   BaseClass::Deserialize(stream);
}

CString HealSpellEffect::ToString() const throw()
{
   CString cszText;

   cszText.Format(IsInstant() ? _T("heal instant") : _T("heal over-time %us tick-time %us"),
      m_uiDuration, m_uiTickTime);

   return cszText;
}


// ModifyAttrSpellEffect methods

void ModifyAttrSpellEffect::Serialize(ByteStream& stream) const
{
   BaseClass::Serialize(stream);

   // TODO impl.
}

void ModifyAttrSpellEffect::Deserialize(ByteStream& stream)
{
   BaseClass::Deserialize(stream);

   // TODO impl.
}

CString ModifyAttrSpellEffect::ToString() const throw()
{
   CString cszText;

   // TODO
   cszText.Format(_T("modify"));

   return cszText;
}


// ResistSpellEffect methods

void ResistSpellEffect::Serialize(ByteStream& stream) const
{
   BaseClass::Serialize(stream);

   // TODO impl.
}

void ResistSpellEffect::Deserialize(ByteStream& stream)
{
   BaseClass::Deserialize(stream);

   // TODO impl.
}

CString ResistSpellEffect::ToString() const throw()
{
   CString cszText;

   // TODO
   cszText.Format(_T("resist"));

   return cszText;
}


// DisableSpellEffect methods

void DisableSpellEffect::Serialize(ByteStream& stream) const
{
   BaseClass::Serialize(stream);

   stream.Write8(static_cast<unsigned char>(m_enDisableType));
}

void DisableSpellEffect::Deserialize(ByteStream& stream)
{
   BaseClass::Deserialize(stream);

   unsigned char ucDisableType = stream.Read8();
   if (ucDisableType >= disableMax)
      throw Exception(_T("invalid disable spell effect type"), __FILE__, __LINE__);

   m_enDisableType = static_cast<T_enDisableType>(ucDisableType);
}

CString DisableSpellEffect::ToString() const throw()
{
   CString cszText;

   LPCTSTR pszDisableType = _T("???");

   switch (m_enDisableType)
   {
   case disableMovement: pszDisableType = _T("movement"); break;
   case disableSpellcast: pszDisableType = _T("spellcast"); break;
   case disableMeleeDistance: pszDisableType = _T("melee+distance"); break;
   case disableMelee: pszDisableType = _T("melee"); break;
   case disableDistance: pszDisableType = _T("distance"); break;
   case disableAll: pszDisableType = _T("all"); break;
   default:
      ATLASSERT(false);
   };

   cszText.Format(_T("disable %s"), pszDisableType);

   return cszText;
}

DisableSpellEffect::T_enDisableType DisableSpellEffect::DisableTypeFromString(LPCTSTR pszDisableType)
{
   if (0 == _tcscmp(pszDisableType, _T("movement"))) return disableMovement;
   if (0 == _tcscmp(pszDisableType, _T("spellcast"))) return disableSpellcast;
   if (0 == _tcscmp(pszDisableType, _T("melee+distance"))) return disableMeleeDistance;
   if (0 == _tcscmp(pszDisableType, _T("melee"))) return disableMelee;
   if (0 == _tcscmp(pszDisableType, _T("distance"))) return disableDistance;
   if (0 == _tcscmp(pszDisableType, _T("all"))) return disableAll;
   throw Exception(_T("invalid disable spell effect type"), __FILE__, __LINE__);
}
