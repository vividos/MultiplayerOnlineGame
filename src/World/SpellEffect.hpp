//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SpellEffect.hpp Spell effect
//
#pragma once

// includes
#include "World.hpp"
#include "DamageType.hpp"

// forward references
class ByteStream;

/// spell effect value or range
class WORLD_DECLSPEC EffectValueOrRange
{
public:
   /// ctor
   EffectValueOrRange()
      :m_bRange(false),
       m_bPercent(false)
   {
      m_iValues[0] = m_iValues[1] = 0;
   }

   // get methods

   /// indicates if effect values is a range
   bool IsRange() const throw() { return m_bRange; }

   /// indicates if effect values or range is in percent
   bool IsPercent() const throw() { return m_bPercent; }

   /// returns value or range start
   int Value() const throw() { return m_iValues[0]; }

   /// returns range end
   int RangeEnd() const throw()
   {
      ATLASSERT(IsRange() == true);
      return m_iValues[1];
   }

   // set methods

   /// sets new value
   void SetValue(bool bPercent, int iValue) throw()
   {
      m_bRange = false;
      m_bPercent = bPercent;
      m_iValues[0] = iValue;
   }

   /// sets new range
   void SetRange(bool bPercent, int iValue1, int iValue2) throw()
   {
      m_bRange = true;
      m_bPercent = bPercent;
      m_iValues[0] = iValue1;
      m_iValues[1] = iValue2;
   }

   // serialize

   /// serialize message by putting bytes to stream
   void Serialize(ByteStream& stream) const;

   /// deserialize message by reading bytes from stream
   void Deserialize(ByteStream& stream);

   /// formats value/range as string
   CString ToString() const throw();

private:
   /// indicates if we have a range
   bool m_bRange;

   /// indicates if values are in percent
   bool m_bPercent;

   /// values
   int m_iValues[2];
};


/// \brief spell effect base class
/// \details not that not only spells can have spell effects, these can also
/// be caused by potions, items, etc.
class WORLD_DECLSPEC SpellEffect
{
public:
   /// spell effect type
   enum T_enEffectType
   {
      typeDamage = 0, ///< damage effect
      typeHeal = 1,   ///< heal effect
      typeModify = 2, ///< modify attribute effect
      typeResist = 3, ///< resist buff effect
      typeDisable = 4,///< disable of ability effect
      //typeSpawn = 5,  ///< spawn mobile effect
      //typeTeleport = 6, ///< teleport effect
      typeMax ///< max. type value
   };

   /// ctor
   SpellEffect(T_enEffectType enEffectType)
      :m_enEffectType(enEffectType)
   {
   }

   // get methods

   /// returns effect type
   T_enEffectType EffectType() const throw() { return m_enEffectType; }

   /// returns effect value / range
   EffectValueOrRange ValueOrRange() const throw() { return m_valueOrRange; }

   // set methods

   /// sets effect value / range
   void ValueOrRange(const EffectValueOrRange& valueOrRange) throw() { m_valueOrRange = valueOrRange; }


   // serialize

   /// constructs new object from stream
   static std::shared_ptr<SpellEffect> Construct(ByteStream& stream);

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream);

   /// formats effect as string
   virtual CString ToString() const throw() = 0;

private:
   /// effect type
   T_enEffectType m_enEffectType;

   /// value/range
   EffectValueOrRange m_valueOrRange;
};


/// base class for damage/heal instant/over time effects
class WORLD_DECLSPEC DamageHealSpellEffectBase: public SpellEffect
{
   typedef SpellEffect BaseClass;

public:
   DamageHealSpellEffectBase(T_enEffectType enEffectType)
      :SpellEffect(enEffectType),
      m_bIsInstant(true),
      m_uiDuration(0),
      m_uiTickTime(0)
   {
      ATLASSERT(enEffectType == typeDamage || enEffectType == typeHeal);
   }

   // get methods

   /// returns if effect is instant (when false, it's over time)
   bool IsInstant() const throw() { return m_bIsInstant; }

   /// when not instant, returns spell duration in seconds
   unsigned int Duration() const throw()
   {
      ATLASSERT(IsInstant() == false);
      return m_uiDuration;
   }

   /// when not instant, returns effect tick time in seconds
   unsigned int TickTime() const throw()
   {
      ATLASSERT(IsInstant() == false);
      return m_uiTickTime;
   }

   // set methods

   /// sets if effect is instant
   void Instant(bool bInstant) throw() { m_bIsInstant = bInstant; }

   /// sets spell duration in seconds
   void Duration(unsigned int uiDuration) throw()
   {
      ATLASSERT(IsInstant() == false);
      m_uiDuration = uiDuration;
   }

   /// sets effect tick time in seconds
   void TickTime(unsigned int uiTickTime) throw()
   {
      ATLASSERT(IsInstant() == false);
      m_uiTickTime = uiTickTime;
   }

   // serialize

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override;

protected:
   bool m_bIsInstant;         ///< indicates if damage is instant or over time
   unsigned int m_uiDuration; ///< duration of damage, when not instant
   unsigned int m_uiTickTime; ///< tick time of damage, when not instant
};


/// spell effect for damage / damage over time (DoT)
class WORLD_DECLSPEC DamageSpellEffect: public DamageHealSpellEffectBase
{
   typedef DamageHealSpellEffectBase BaseClass;

public:
   /// ctor
   DamageSpellEffect() throw()
      :DamageHealSpellEffectBase(SpellEffect::typeDamage),
       m_ucRestriction(0)
   {
   }

   /// restrictions to applying this damage
   enum T_enDamageEffectRestriction
   {
      noArmor = 0, ///< no armor is checked
      noParryBlock = 1, ///< cannot be parried or blocked
   };

   // get methods

   /// returns damage type
   DamageType GetDamageType() const throw() { return m_damageType; }

   /// returns restriction setting
   bool Restriction(T_enDamageEffectRestriction enRestriction) const;

   // set method

   /// sets damage type
   void SetDamageType(const DamageType& damageType) throw() { m_damageType = damageType; }

   /// sets restriction flag
   void Restriction(T_enDamageEffectRestriction enRestriction, bool bRestrict);

   // serialize

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override;

   /// formats effect as string
   virtual CString ToString() const throw() override;

private:
   /// damage type
   DamageType m_damageType;

   /// restriction flags
   unsigned char m_ucRestriction;
};

/// spell effect for heal / heal over time (HoT)
class WORLD_DECLSPEC HealSpellEffect: public DamageHealSpellEffectBase
{
   typedef DamageHealSpellEffectBase BaseClass;

public:
   /// ctor
   HealSpellEffect() throw()
      :DamageHealSpellEffectBase(SpellEffect::typeHeal)
   {
   }

   // serialize

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override;

   /// formats effect as string
   virtual CString ToString() const throw() override;
};

/// spell effect that modifies attributes in player that has that buff/debuff
/// \todo implement
class WORLD_DECLSPEC ModifyAttrSpellEffect: public SpellEffect
{
   typedef SpellEffect BaseClass;

public:
   /// ctor
   ModifyAttrSpellEffect() throw()
      :SpellEffect(SpellEffect::typeModify)
   {
   }

   // serialize

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override;

   /// formats effect as string
   virtual CString ToString() const throw() override;

private:
};

/// spell effect that resists attacks or spells
/// \todo implement
class WORLD_DECLSPEC ResistSpellEffect: public SpellEffect
{
   typedef SpellEffect BaseClass;

public:
   /// ctor
   ResistSpellEffect() throw()
      :SpellEffect(SpellEffect::typeResist)
   {
   }

   // serialize

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override;

   /// formats effect as string
   virtual CString ToString() const throw() override;

private:
};

/// spell effect that disables abilities of the mobile
class WORLD_DECLSPEC DisableSpellEffect: public SpellEffect
{
   typedef SpellEffect BaseClass;

public:
   /// type of disabled action
   enum T_enDisableType
   {
      disableMovement = 0,       ///< disables movement
      disableSpellcast = 1,      ///< disables spellcasting
      disableMeleeDistance = 2,  ///< disables melee and distance attacks
      disableMelee = 3,          ///< disables melee attacks
      disableDistance = 4,       ///< disables distance attacks
      disableAll = 5,            ///< disables all of the above
      disableMax ///< max. value
   };

   /// ctor
   DisableSpellEffect() throw()
      :SpellEffect(SpellEffect::typeDisable),
      m_enDisableType(disableAll)
   {
   }

   // get methods

   /// returns disable type
   T_enDisableType DisableType() const throw() { return m_enDisableType; }

   // set methods

   /// sets new disable type
   void DisableType(T_enDisableType enDisableType) throw() { m_enDisableType = enDisableType; }

   // serialize

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override;

   /// formats effect as string
   virtual CString ToString() const throw() override;

   /// returns disable type enum by string
   static T_enDisableType DisableTypeFromString(LPCTSTR pszDisableType);

private:
   /// disable type
   T_enDisableType m_enDisableType;
};
