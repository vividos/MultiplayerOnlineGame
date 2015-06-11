//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2012 Michael Fink
//
//! \file RuleSystem.cpp Game rule system
//

// includes
#include "StdAfx.h"
#include "RuleSystem.hpp"

RuleSystem::RuleSystem()
{
}

double RuleSystem::Random(double /*dMin*/, double /*dMax*/)
{
   // TODO impl.
   return 0.0;
}

double RuleSystem::Gaussian(double /*dSigma*/)
{
   // TODO impl.
   return 0.0;
}

double RuleSystem::Clamp(double /*dValue*/, double /*dMin*/, double /*dMax*/)
{
   // TODO impl.
   return 0.0;
}


void RuleSystem::DoAttack(const Attack& /*attack*/,
   std::vector<ActionPtr>& /*vecResultActions*/)
{
}

void RuleSystem::Cast(MobilePtr /*spCaster*/, const Spell& /*spell*/,
   std::vector<ActionPtr>& /*vecResultActions*/)
{
}
