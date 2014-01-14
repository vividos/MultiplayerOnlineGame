//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2012 Michael Fink
//
//! \file RuleSystem.hpp Game rule system
//
#pragma once

// includes
#include <vector>
#include "Mobile.hpp"
#include "Item.hpp"
#include "Action.hpp"

// forward references
class Spell;

class AttackInfo
{
public:
   MobilePtr m_spAttacker;
   MobilePtr m_spTarget;
};



/// \brief game rule system
/// \details handles basic user and mobile actions and generates Action objects
class RuleSystem
{
public:
   /// ctor
   RuleSystem();

   /// performs a physical attack
   void DoAttack(const AttackInfo& attack,
      std::vector<ActionPtr>& vecResultActions);

   /// casts a spell
   void Cast(MobilePtr spCaster, const Spell& spell,
      std::vector<ActionPtr>& vecResultActions);

private:
   static double Random(double dMin, double dMax);
   static double Gaussian(double dSigma);
   static double Clamp(double dValue, double dMin, double dMax);
};
