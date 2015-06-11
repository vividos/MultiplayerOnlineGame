//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IPlayerViewModel.hpp interface for access to player view-model
//
#pragma once

// includes
#include "Vector3.hpp"

// forward references
class MovementInfo;

/// interface to player in ViewModel
class IPlayerViewModel
{
public:
   /// dtor
   virtual ~IPlayerViewModel() throw() {}

   // get methods

   /// returns player position
   virtual const Vector3d& GetPlayerPosition() const = 0;

   /// returns player movement angle
   virtual double GetPlayerMovementAngle() const = 0;

   /// returns if player view angle is controllable
   virtual bool IsPlayerViewAngleInControl() const = 0;

   // set methods

   /// sets player transparency
   virtual void SetPlayerTransparency(double dTransparency) = 0;

   // actions

   /// updates player movement
   virtual void UpdatePlayerMovement(const MovementInfo& movementInfo) = 0;

   /// updates player position by evaluating movement info
   virtual void UpdatePlayerPos() = 0;

   /// updates player view angle
   virtual void UpdatePlayerViewAngle(double dViewAngle) = 0;
};
