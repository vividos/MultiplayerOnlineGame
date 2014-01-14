//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Spell.cpp Spell class
//

// includes
#include "StdAfx.h"
#include "Spell.hpp"
#include "SpellEffect.hpp"
#include "ByteStream.hpp"
#include "Flags8.hpp"

std::shared_ptr<Spell> Spell::Construct(ByteStream& stream)
{
   std::shared_ptr<Spell> spSpell(new Spell(stream));
   spSpell->Deserialize(stream);

   return spSpell;
}

void Spell::Serialize(ByteStream& stream) const
{
   ATLASSERT(m_uiSpellId <= std::numeric_limits<unsigned short>::max());
   stream.Write16(static_cast<unsigned short>(m_uiSpellId));

   Flags8 flags;
   flags.SetBit<0>(m_uiCooldown != 0);
   flags.SetBit<1>(m_uiCastTime != 0);
   flags.SetBit<2>(m_uiRange != 0);
   flags.SetBit<3>(m_bAreaSpell);
   flags.SetBit<4>(m_bStackable);
   flags.SetBit<5>(HasEffect2());

   stream.Write8(flags.Value());

   if (m_uiCooldown != 0)
      stream.Write16(static_cast<unsigned short>(m_uiCooldown));
   if (m_uiCastTime != 0)
      stream.Write16(static_cast<unsigned short>(m_uiCastTime));
   if (m_uiRange != 0)
      stream.Write16(static_cast<unsigned short>(m_uiRange));

   m_spEffect1->Serialize(stream);

   if (HasEffect2())
      m_spEffect2->Serialize(stream);
}

void Spell::Deserialize(ByteStream& stream)
{
   m_uiSpellId = stream.Read16();

   Flags8 flags = stream.Read8();

   if (flags.GetBit<0>())
      m_uiCooldown = stream.Read16();

   if (flags.GetBit<1>())
      m_uiCastTime = stream.Read16();
   if (flags.GetBit<2>())
      m_uiRange = stream.Read16();

   m_bAreaSpell = flags.GetBit<3>();
   m_bStackable = flags.GetBit<4>();

   m_spEffect1 = SpellEffect::Construct(stream);

   if (flags.GetBit<5>())
      m_spEffect2 = SpellEffect::Construct(stream);
   else
      m_spEffect2.reset();
}

CString Spell::ToString() const throw()
{
   CString cszText, cszTemp;

   cszText.Format(_T("spell %04x"), m_uiSpellId);

   if (m_uiCooldown > 0)
   {
      cszTemp.Format(_T(" cooldown %us"), m_uiCooldown);
      cszText += cszTemp;
   }

   if (m_uiCastTime > 0)
   {
      cszTemp.Format(_T(" cast-time %us"), m_uiCastTime);
      cszText += cszTemp;
   }

   if (m_bAreaSpell)
      cszText += _T(" area");

   if (m_uiRange > 0)
   {
      cszTemp.Format(_T(" range %um"), m_uiRange);
      cszText += cszTemp;
   }

   if (m_bStackable)
      cszText += _T(" stackable");

   cszText += _T(" effect [") + m_spEffect1->ToString() + _T("]");

   if (m_spEffect2 != NULL)
      cszText += _T(" effect [") + m_spEffect2->ToString() + _T("]");

   return cszText;
}
