//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file MovementInfo.hpp Movement info
//
#pragma once

// includes
#include "Common.hpp"
#include "Vector3d.hpp"
#include "Timer.hpp"

// forward references
class ByteStream;

/// movement info to predict further movement of mobile or player
class COMMON_DECLSPEC MovementInfo
{
public:
   /// movement mode
   enum T_enMovementMode
   {
      movementStand,       ///< not moving
      movementTarget,      ///< moving to target
      movementDirection,   ///< moving in direction
      movementPlayer,      ///< complex player movement

      movementMax = movementPlayer
   };

   /// movement type
   enum T_enMovementType
   {
      moveTypeStand,       ///< standing
      moveTypeWalkForward, ///< walking forward
      moveTypeWalkBackward,///< walking backwards
      moveTypeJump,        ///< jumping
      moveTypeFall,        ///< falling
      moveTypeSwim,        ///< swimming
      moveTypeDie,         ///< dieing
   };

   /// ctor
   MovementInfo(T_enMovementMode enMovementMode = movementStand) throw();

   // getter

   /// returns movement mode
   T_enMovementMode MovementMode() const throw() { return m_enMovementMode; }

   /// returns movement type
   T_enMovementType MovementType() const throw();

   /// returns position at start of update of movement info
   const Vector3d& Position() const throw() { return m_vCurPos; }

   /// returns destination position
   const Vector3d& Destination() const throw() { return m_vDestPos; }

   /// returns speed
   double Speed() const throw() { return m_dSpeedInUnitsPerSec; }

   /// returns view angle to destination
   double ViewAngle() const throw();

   // setter

   /// sets current position
   void Position(const Vector3d& pos) throw() { m_vCurPos = pos; }

   /// sets destination coordinates
   void Destination(Vector3d destPos) throw() { m_vDestPos = destPos; }

   /// sets movement direction; for movementDirection mode
   void Direction(double dDirection) throw() { m_dDirection = dDirection; }

   /// sets speed
   void Speed(double dSpeedInUnitsPerSec) throw() { m_dSpeedInUnitsPerSec = dSpeedInUnitsPerSec; }

   /// sets forward movement (forward or backward)
   void SetForwardMovement(bool bMovement, bool bMoveForward);

   /// sets sideways movement (left or right)
   void SetSidewaysMovement(bool bMovement, bool bMoveLeft);

   // actions

   /// calculates position based on movement info and elapsed time
   Vector3d PredictPosition(double dElapsedTime) const;

   /// calculates time to reach destination, in seconds
   double TimeToReachDestination() const;

   // serialize

   /// serialize message by putting bytes to stream
   void Serialize(ByteStream& stream) const;

   /// deserialize message by reading bytes from stream
   void Deserialize(ByteStream& stream);

   // operators

   /// compare operator
   bool operator==(const MovementInfo& rhs) const throw();

   // inequality
   bool operator!=(const MovementInfo& rhs) const throw()
   {
      return !operator==(rhs);
   }

private:
   /// movement mode
   T_enMovementMode m_enMovementMode;

   /// current position
   Vector3d m_vCurPos;

   /// destination position
   Vector3d m_vDestPos;

   /// movement direction
   double m_dDirection;

   /// speed in units/s
   double m_dSpeedInUnitsPerSec;

   /// movement start
   Timer m_timerStartMovement;

   bool m_bForwardMovement;   ///< forward/backward movement active?
   bool m_bMoveForward;       ///< true: forward, false: backward
   bool m_bSidewaysMovement;  ///< sideways movement active?
   bool m_bSidewaysMoveLeft;  ///< true: left, false: right
};
