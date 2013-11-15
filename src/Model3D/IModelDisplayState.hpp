//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file IModelDisplayState.hpp model display state interface
//
#pragma once

// forward references
class MovementInfo;
class RenderOptions;

/// model animation type
enum T_enModelAnimation
{
   animIdle1 = 0,    ///< idle animation, variant 1
   animIdle2,        ///< idle animation, variant 2
   animIdle3,        ///< idle animation, variant 3

   animIdleCombat,   ///< idle, in combat mode
   animRunCombat,    ///< run, in combat mode
   animAttack1,      ///< attack, variant 1
   animAttack2,      ///< attack, variant 2
   animHit,          ///< being hit animation

   animRunning,      ///< running animation
   animWalking,      ///< walking animation
   animMoveBackwards,///< moving backwards animation
   animMoveSide,     ///< moving sideways
   animJumping,      ///< jumping
   animFalling,      ///< falling
   animSwimming,     ///< swimming

   animUnlockDoor,   ///< unlocking door (in front)
   animUnlockChest,  ///< unlocking chest (at floor)
   animPickUp,       ///< picking up from floor animation
   animDrinkPotion,  ///< drink potion animation

   animSleep,        ///< sleeping on the floor

   animDeath1,       ///< death animation, variant 1
   animDeath2,       ///< death animation, variant 2
   animDeath3,       ///< death animation, variant 3

   animEmoteGreet,   ///< greet emote
   animEmoteLaugh,   ///< laugh emote
   animEmoteYes,     ///< yes emote
   animEmoteNo,      ///< no emote
   animEmoteApplaud, ///< applaud emote

   animMax ///< max. animation
};

/// type of weapon that the model holds, for animations
enum T_enModelAnimationWeaponType
{
   animWeapon1handed,
   animWeapon2handed,
   animWeaponBow,
   animWeaponCrossbow,
   animWeaponHand,
};


/// model display state
class IModelDisplayState
{
public:
   /// dtor
   virtual ~IModelDisplayState() throw() {}

   /// tick calculation
   virtual void Tick(double dElapsed) = 0;

   /// sets new animation
   virtual void SetAnimation(T_enModelAnimation enAnimation, T_enModelAnimationWeaponType enAnimWeaponType,
      bool bLoop, bool bInterruptable) = 0;

   /// sets new animation frames
   virtual void SetAnimationFrames(unsigned int uiStartFrame, unsigned int uiEndFrame,
      bool bLoop, bool bInterruptable) = 0;

   /// update model movement
   virtual void UpdateMovement(const MovementInfo& info) = 0;

   /// renders model instance
   virtual void Render(RenderOptions& options) = 0;
};
