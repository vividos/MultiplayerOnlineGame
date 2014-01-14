//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file Spell.hpp Spell class
//
#pragma once

// includes
#include "World.hpp"

// forward references
class ByteStream;
class SpellEffect;

/// \brief spell
/// \details combines one or two spell effects
class WORLD_DECLSPEC Spell
{
public:
   /// ctor
   Spell(unsigned int uiSpellId, std::shared_ptr<SpellEffect> spEffect1)
      :m_uiSpellId(uiSpellId),
       m_uiCooldown(0),
       m_uiCastTime(0),
       m_uiRange(0),
       m_bAreaSpell(false),
       m_bStackable(false),
       m_spEffect1(spEffect1)
   {
      ATLASSERT(spEffect1 != nullptr); // must provide an effect
   }

   // get methods

   /// returns spell id
   unsigned int Id() const throw() { return m_uiSpellId; }

   /// returns cooldown time in seconds; 0 means off
   unsigned int Cooldown() const throw() { return m_uiCooldown; }

   /// returns cast time in seconds; 0 means instant cast
   unsigned int CastTime() const throw() { return m_uiCastTime; }

   /// range needed to cast spell on; 0 means no range checks performed
   unsigned int Range() const throw() { return m_uiRange; }

   /// indicates if it's an area spell; then Range() defines area radius
   bool IsAreaSpell() const throw() { return m_bAreaSpell; }

   /// indicates if effect is stackable
   bool IsStackable() const throw() { return m_bStackable; }

   /// indicates if spell has Effect2, too (Effect1 is always set)
   bool HasEffect2() const throw()
   {
      return m_spEffect2 != nullptr;
   }

   /// returns first effect
   const SpellEffect& Effect1() const throw()
   {
      ATLASSERT(m_spEffect1 != nullptr);
      return *m_spEffect1;
   }

   /// returns second effect
   const SpellEffect& Effect2() const throw()
   {
      ATLASSERT(HasEffect2() == true);
      return *m_spEffect2;
   }

   // set methods

   /// sets cooldown time in seconds; 0 means off
   void Cooldown(unsigned int uiCooldown) throw() { m_uiCooldown = uiCooldown; }

   /// sets cast time in seconds; 0 means instant cast
   void CastTime(unsigned int uiCastTime) throw() { m_uiCastTime = uiCastTime; }

   /// sets range needed to cast spell on
   void Range(unsigned int uiRange) throw() { m_uiRange = uiRange; }

   /// sets if it's an area spell
   void AreaSpell(bool bAreaSpell) throw() { m_bAreaSpell = bAreaSpell; }

   /// sets if effect is stackable
   void Stackable(bool bStackable) throw() { m_bStackable = bStackable; }

   /// sets second effect
   void Effect2(std::shared_ptr<SpellEffect> spEffect2) throw() { m_spEffect2 = spEffect2; }

   // serialize

   /// constructs new object from stream
   static std::shared_ptr<Spell> Construct(ByteStream& stream);

   /// serialize message by putting bytes to stream
   void Serialize(ByteStream& stream) const;

   /// deserialize message by reading bytes from stream
   void Deserialize(ByteStream& stream);

   /// formats value/range as string
   CString ToString() const throw();

private:
   /// private ctor; used by Construct()
   Spell(ByteStream&)
      :m_uiSpellId(0),
       m_uiCooldown(0),
       m_uiCastTime(0),
       m_uiRange(0),
       m_bAreaSpell(false),
       m_bStackable(false)
   {
   }

private:
   unsigned int m_uiSpellId;  ///< spell id
   unsigned int m_uiCooldown; ///< cooldown time in seconds
   unsigned int m_uiCastTime; ///< cast time in seconds
   unsigned int m_uiRange;    ///< range of spell in game units
   bool m_bAreaSpell;         ///< is area spell?
   bool m_bStackable;         ///< is stackable?
   std::shared_ptr<SpellEffect> m_spEffect1;  ///< first spell effect
   std::shared_ptr<SpellEffect> m_spEffect2;  ///< second spell effect (may be null)
};
