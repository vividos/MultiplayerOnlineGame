//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AnimationData.cpp Animation data for Warriors & Commoners model pack
//

// includes
#include "StdAfx.h"
#include "AnimationData.hpp"
#include <boost/foreach.hpp>

using MilkShape3D::WarriorsAndCommoners::AnimationData;

/// animation data
struct AnimData
{
   LPCTSTR pszName;           ///< animation name
   unsigned int uiStartFrame; ///< start frame
   unsigned int uiEndFrame;   ///< end frame
};

/// animation data for the CS Warriors & Commoners .ms3d models
AnimData g_aAnimData[] =
{
   { _T("1h_attack1"), 0, 20 },     // attack - carrying a 1 handed weapon
   { _T("1h_attack2"), 21, 41 },    // attack - carrying a 1 handed weapon
   { _T("1h_hit"), 42, 57 },        // hit - carrying a 1 handed weapon
   { _T("1h_idle"), 58, 98 },       // idle - carrying a 1 handed weapon
   { _T("1h_parade"), 99, 104 },    // parade pose - carrying a 1 handed weapon
   { _T("1h_run"), 105, 135 },      // run - carrying a 1 handed weapon
   { _T("2h_attack1"), 136, 156 },  // attack - carrying a 2 handed weapon
   { _T("2h_attack2"), 157, 177 },  // attack - carrying a 2 handed weapon
   { _T("2h_hit"), 178, 193 },      // hit - carrying a 2 handed weapon
   { _T("2h_idle"), 194, 234 },     // idle - carrying a 2 handed weapon
   { _T("2h_run"), 235, 265 },      // run - carrying a 2 handed weapon
   { _T("action1"), 266, 316 },     // siting down
   { _T("action10"), 317, 417 },    // waiting, scrape the thigh
   { _T("action2"), 418, 568 },     // sat, conversation
   { _T("action3"), 569, 749 },     // sat, drink
   { _T("action4"), 750, 800 },     // geting up
   { _T("action5"), 801, 841 },     // unlocking door
   { _T("action6"), 842, 882 },     // unlocking chest
   { _T("action7"), 883, 1033 },    // sat, discussion
   { _T("action8"), 1034, 1094 },   // pick up
   { _T("action9"), 1095, 1195 },   // drink potion
   { _T("back"), 1196, 1236 },      // move back
   { _T("bow_attack"), 1237, 1297 },// attack - using a bow
   { _T("bow_hit"), 1298, 1313 },   // hit - carrying a bow
   { _T("bow_idle"), 1314, 1354 },  // idle - carrying a bow
   { _T("crossbow_fire"), 1355, 1365 },   // attack - using a crossbow
   { _T("crossbow_hit"), 1366, 1386 },    // hit - carrying a crossbow
   { _T("crossbow_idle"), 1387, 1427 },   // idle - carrying a crossbow
   { _T("crossbow_run"), 1428, 1458 },    // run - carrying a crossbow
   { _T("dead1"), 1459, 1519 },     // death
   { _T("dead2"), 1520, 1560 },     // death
   { _T("dead3"), 1561, 1611 },     // death
   { _T("emote1"), 1612, 1692 },    // yes
   { _T("emote10"), 1693, 1793 },   // crying
   { _T("emote11"), 1794, 1944 },   // frightened 
   { _T("emote12"), 1945, 2120 },   // discussing
   { _T("emote2"), 2121, 2201 },    // no
   { _T("emote3"), 2202, 2302 },    // discussing
   { _T("emote4"), 2303, 2383 },    // laughing
   { _T("emote5"), 2384, 2464 },    // victory gesture
   { _T("emote6"), 2465, 2545 },    // noble salute
   { _T("emote7"), 2546, 2626 },    // salute
   { _T("emote8"), 2627, 2687 },    // protesting
   { _T("emote9"), 2688, 2788 },    // applaud 
   { _T("fall"), 2789, 2819 },      // falling
   { _T("H2H_hit"), 2820, 2835 },   // hit - hand to hand fight
   { _T("H2H_idle"), 2836, 2876 },  // idle - hand to hand fight
   { _T("H2H_punch"), 2877, 2887 }, // punch - hand to hand fight
   { _T("idle1"), 2888, 2928 },     // waiting
   { _T("idle10"), 2929, 2999 },    // peasant at work
   { _T("idle11"), 3000, 3050 },    // blacksmith at work
   { _T("idle12"), 3051, 3251 },    // look around
   { _T("idle13"), 3252, 3282 },    // mining
   { _T("idle14"), 3283, 3323 },    // waiting, carrying a bag
   { _T("idle15"), 3324, 3364 },    // waiting, carrying a wood pile
   { _T("idle16"), 3365, 3405 },    // waiting, carrying a torch
   { _T("idle17"), 3406, 3456 },    // chopping
   { _T("idle2"), 3457, 3487 },     // blown
   { _T("idle3"), 3488, 3528 },     // with one's arms folded
   { _T("idle4"), 3529, 3629 },     // sleeping
   { _T("idle5"), 3630, 3680 },     // sat
   { _T("idle6"), 3681, 3731 },     // sat with tankard
   { _T("idle7"), 3732, 3773 },     // innkeeper with plate 
   { _T("idle8"), 3773, 3854 },     // merchant waiting
   { _T("idle9"), 3854, 4054 },     // beggar
   { _T("jump"), 4055, 4080 },      // jumping
   { _T("run"), 4081, 4111 },       // runing
   { _T("side"), 4112, 4132 },      // move side
   { _T("swim"), 4133, 4173 },      // swiming
   { _T("walk1"), 4174, 4214 },     // walking
   { _T("walk2"), 4215, 4275 },     // furtive walking
   { _T("walk3"), 4276, 4306 },     // walking with plate
   { _T("walk4"), 4307, 4347 },     // object on shoulder
   { _T("walk5"), 4348, 4388 },     // walking and carrying a bag
   { _T("walk6"), 4389, 4429 },     // walking and carrying a wood pile
   { _T("walk7"), 4430, 4471 },     // walking and carrying a torch
};

void AnimationData::EnumAnimations(std::vector<CString>& vecAnimations)
{
   BOOST_FOREACH(const AnimData& data, g_aAnimData)
      vecAnimations.push_back(data.pszName);
}

void AnimationData::GetAnimationFrames(const CString& cszAnimation, unsigned int& uiStartFrame, unsigned int& uiEndFrame)
{
   BOOST_FOREACH(const AnimData& data, g_aAnimData)
      if (data.pszName == cszAnimation)
      {
         uiStartFrame = data.uiStartFrame;
         uiEndFrame = data.uiEndFrame;
         return;
      }
}

void AnimationData::GetAnimationFrames(T_enModelAnimation enAnimation,
                                       T_enModelAnimationWeaponType enAnimWeaponType,
                                       unsigned int& uiStartFrame, unsigned int& uiEndFrame)
{
   LPCTSTR pszAnimation = _T("");
   switch(enAnimation)
   {
   case animIdle1:   pszAnimation = _T("idle1"); break;
   case animIdle2:   pszAnimation = _T("idle12"); break;
   case animIdle3:   pszAnimation = _T("action10"); break;

   case animIdleCombat:
      switch(enAnimWeaponType)
      {
      case animWeapon1handed: pszAnimation = _T("1h_idle"); break;
      case animWeapon2handed: pszAnimation = _T("2h_idle"); break;
      case animWeaponBow:     pszAnimation = _T("bow_idle"); break;
      case animWeaponCrossbow:pszAnimation = _T("crossbow_idle"); break;
      case animWeaponHand:    pszAnimation = _T("H2H_idle"); break;
      }
      break;

   case animRunCombat:
      switch(enAnimWeaponType)
      {
      case animWeapon1handed: pszAnimation = _T("1h_run"); break;
      case animWeapon2handed: pszAnimation = _T("2h_run"); break;
      case animWeaponBow:     pszAnimation = _T("run"); break; // note: no special anim available
      case animWeaponCrossbow:pszAnimation = _T("crossbow_run"); break;
      case animWeaponHand:    pszAnimation = _T("run"); break; // note: no special anim available
      }
      break;

   case animAttack1:
      switch(enAnimWeaponType)
      {
      case animWeapon1handed: pszAnimation = _T("1h_attack1"); break;
      case animWeapon2handed: pszAnimation = _T("2h_attack1"); break;
      case animWeaponBow:     pszAnimation = _T("bow_attack"); break;
      case animWeaponCrossbow:pszAnimation = _T("crossbow_fire"); break;
      case animWeaponHand:    pszAnimation = _T("H2H_punch"); break;
      }
      break;

   case animAttack2:
      switch(enAnimWeaponType)
      {
      case animWeapon1handed: pszAnimation = _T("1h_attack2"); break;
      case animWeapon2handed: pszAnimation = _T("2h_attack2"); break;
      case animWeaponBow:     pszAnimation = _T("bow_attack"); break;
      case animWeaponCrossbow:pszAnimation = _T("crossbow_fire"); break;
      case animWeaponHand:    pszAnimation = _T("H2H_punch"); break;
      }
      break;

   case animHit:
      switch(enAnimWeaponType)
      {
      case animWeapon1handed: pszAnimation = _T("1h_hit"); break;
      case animWeapon2handed: pszAnimation = _T("2h_hit"); break;
      case animWeaponBow:     pszAnimation = _T("bow_hit"); break;
      case animWeaponCrossbow:pszAnimation = _T("crossbow_hit"); break;
      case animWeaponHand:    pszAnimation = _T("H2H_hit"); break;
      }
      break;

   case animRunning:       pszAnimation = _T("run"); break;
   case animWalking:       pszAnimation = _T("walk1"); break;
   case animMoveBackwards: pszAnimation = _T("back"); break;
   case animMoveSide:      pszAnimation = _T("side"); break;
   case animJumping:       pszAnimation = _T("jump"); break;
   case animFalling:       pszAnimation = _T("fall"); break;
   case animSwimming:      pszAnimation = _T("swim"); break;

   case animUnlockDoor:    pszAnimation = _T("action5"); break;
   case animUnlockChest:   pszAnimation = _T("action6"); break;
   case animPickUp:        pszAnimation = _T("action8"); break;
   case animDrinkPotion:   pszAnimation = _T("action9"); break;

   case animSleep:         pszAnimation = _T("idle4"); break;

   case animDeath1:        pszAnimation = _T("dead1"); break;
   case animDeath2:        pszAnimation = _T("dead2"); break;
   case animDeath3:        pszAnimation = _T("dead3"); break;

   case animEmoteGreet:    pszAnimation = _T("emote7"); break;
   case animEmoteLaugh:    pszAnimation = _T("emote4"); break;
   case animEmoteYes:      pszAnimation = _T("emote1"); break;
   case animEmoteNo:       pszAnimation = _T("emote2"); break;
   case animEmoteApplaud:  pszAnimation = _T("emote9"); break;
   }

   return GetAnimationFrames(pszAnimation, uiStartFrame, uiEndFrame);
}
