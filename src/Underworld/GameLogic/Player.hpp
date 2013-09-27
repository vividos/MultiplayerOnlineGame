//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/GameLogic/Player.hpp Player info
//
#pragma once

// includes
#include "Inventory.hpp"
#include "Runes.hpp"
#include "ConvGlobals.hpp"
#include "QuestFlags.hpp"

namespace Underworld
{

// enums

/// \brief Player attributes
/// \todo add more needed skills
enum EPlayerAttribute
{
   attrGender = 0,     ///< 0 means male
   attrHandedness = 1, ///< 0 means left-handedness
   attrAppearance = 2, ///< values from 0..4
   attrProfession = 3, ///< values from 0..7, fighter, mage, bard, ...

   attrMapLevel = 4,   ///< map level the player currently is

   attrStrength = 5,
   attrDexterity = 6,
   attrIntelligence = 7,

   attrVitality = 8,
   attrMaxVitality = 9,
   attrMana = 10,
   attrMaxMana = 11,

   attrWeariness = 12,
   attrHungriness = 13,
   attrPoisoned = 14,    ///< 1 when poisoned
   attrMentalState = 15, ///< drunk, tripping, etc. 0 means normal
   attrNightVision = 16,

   attrTalks = 17,       ///< number of conversations
   attrKills = 18,       ///< number of successful kills
   attrExperienceLevel = 19,   ///< experience level
   attrExperiencePoints = 20,  ///< number of experience points

   attrDifficulty = 21,  ///< 0 = easy, 1 = normal

   // todo: game time, etc.

   attrMax
};


/// \brief Player skills
enum EPlayerSkill
{
   skillAttack = 0,
   skillDefense = 1,
   skillUnarmed = 2,
   skillSword = 3,
   skillAxe = 4,
   skillMace = 5,
   skillMissile = 6,
   skillMana = 7,
   skillLore = 8,
   skillCasting = 9,
   skillTraps = 10,
   skillSearch = 11,
   skillTrack = 12,
   skillSneak = 13,
   skillRepair = 14,
   skillCharm = 15,
   skillPicklock = 16,
   skillAcrobat = 17,
   skillAppraise = 18,
   skillSwimming = 19,

   skillMax
};


// classes

/// Player
class Player
{
public:
   /// ctor
   Player();

   // set functions

   /// Sets player position
   void SetPos(double x, double y){ m_xpos = x; m_ypos = y; }

   /// Sets player height
   void SetHeight(double dHeight){ m_dHeight = dHeight; }

   /// Sets player view rotation angle
   void SetRotateAngle(double dAngle){ m_dRotateAngle = dAngle; }

   /// Sets player view panning angle
   void SetPanAngle(double dAngle){ m_dPanAngle = dAngle; }

   /// Sets player attribute value
   void SetAttribute(EPlayerAttribute attr, Uint16 uiValue)
   {
      UaAssert(attr < attrMax);
      m_aAttributes[attr] = uiValue;
   }

   /// Sets player skill value
   void SetSkill(EPlayerSkill skill, Uint16 uiValue)
   {
      UaAssert(skill < skillMax);
      m_aSkills[skill] = uiValue;
   }

   /// Sets player name
   void SetName(const CString& cszName){ m_cszName = cszName; }


   // get functions

   /// Returns x position
   double GetXPos() const { return m_xpos; }

   /// Returns y position
   double GetYPos() const { return m_ypos; }

   /// Returns player height
   double GetHeight() const { return m_dHeight; }

   /// returns view rotation angle
   double GetRotateAngle() const { return m_dRotateAngle; }

   /// returns view panning angle
   double GetPanAngle() const { return m_dPanAngle; }

   /// Returns player name
   const CString& GetName() const { return m_cszName; }

   /// Returns player attribute value
   Uint16 GetAttribute(EPlayerAttribute attr) const
   {
      UaAssert(attr < attrMax);
      return m_aAttributes[attr];
   }

   /// Returns player skill value
   Uint16 GetSkill(EPlayerSkill skill) const
   {
      UaAssert(skill < skillMax);
      return m_aSkills[skill];
   }

   /// Returns inventory
   Inventory& GetInventory(){ return m_inventory; }
   /// Returns inventory
   const Inventory& GetInventory() const { return m_inventory; }

   /// Returns runebag
   Runebag& GetRunebag(){ return m_runebag; }
   /// Returns runebag
   const Runebag& GetRunebag() const { return m_runebag; }

   /// Returns conversation globals
   ConvGlobals& GetConvGlobals(){ return m_convGlobals; }
   /// Returns conversation globals
   const ConvGlobals& GetConvGlobals() const { return m_convGlobals; }

   /// Returns quest flags
   QuestFlags& GetQuestFlags(){ return m_questFlags; }
   /// Returns conversation globals
   const QuestFlags& GetQuestFlags() const { return m_questFlags; }

   // loading / saving

   /// Loads player info from savegame
   void Load(UwBase::Savegame& sg);

   /// Saves player infos to savegame
   void Save(UwBase::Savegame& sg) const;

private:
   /// the name of the player
   CString m_cszName;

   /// positional values
   double m_xpos; ///< x position
   double m_ypos; ///< y position
   double m_dHeight;       ///< height
   double m_dRotateAngle;  ///< rotate angle
   double m_dPanAngle;     ///< view pan angle

   /// array with all player attributes
   Uint16 m_aAttributes[attrMax];

   /// array with all player skills
   Uint16 m_aSkills[skillMax];

   /// player's inventory
   Inventory m_inventory;

   /// runebag
   Runebag m_runebag;

   /// conversation globals
   ConvGlobals m_convGlobals;

   /// quest flags
   QuestFlags m_questFlags;
};

} // namespace Underworld
