//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MovementInfo.cpp Movement info
//

// includes
#include "stdafx.h"
#include "MovementInfo.hpp"
#include "ByteStream.hpp"

/// speed of moving forward, in units/second
const double c_dMoveForwardSpeed = 5.0;

/// speed of moving backward, in units/second
const double c_dMoveBackwardSpeed = 3.0;

/// strafe walking speed, in units/second
const double c_dStrafeSpeed = 3.0;

MovementInfo::MovementInfo(T_enMovementMode enMovementMode)
:m_enMovementMode(enMovementMode),
 m_dDirection(0.0),
 m_dSpeedInUnitsPerSec(0.0),
 m_bForwardMovement(false),
 m_bMoveForward(false),
 m_bSidewaysMovement(false),
 m_bSidewaysMoveLeft(false)
{
   if (m_enMovementMode == movementTarget)
      m_timerStartMovement.Start();
}

MovementInfo::T_enMovementType MovementInfo::MovementType() const throw()
{
   if (m_enMovementMode == movementStand)
      return moveTypeStand;

   if (m_enMovementMode == movementTarget)
   {
      double dElapsed = m_timerStartMovement.Elapsed();
      if (dElapsed < TimeToReachDestination())
         return moveTypeWalkForward;
      else
         return moveTypeStand;
   }

   if (m_enMovementMode == movementDirection)
      return moveTypeWalkForward; // walking forever

   if (m_enMovementMode == movementPlayer)
   {
      if (m_bForwardMovement)
         return m_bMoveForward ? moveTypeWalkForward : moveTypeWalkBackward;

      if (m_bSidewaysMovement)
         return moveTypeWalkForward;

      return moveTypeStand;
   }

   ATLASSERT(false);
   return moveTypeStand;
}

double MovementInfo::ViewAngle() const throw()
{
   if (m_enMovementMode == movementDirection)
      return m_dDirection;

   if (m_enMovementMode == movementTarget)
   {
      // only use x and z
      Vector3d vDir = m_vDestPos - m_vCurPos;
      double dAngle = Rad2Deg(std::atan2(-vDir.Z(), vDir.X()));
      return AngleInRange(dAngle + 90.0);
   }

   if (m_enMovementMode == movementPlayer)
   {
      return m_dDirection;
   }

   ATLASSERT(false);
   return 0.0;
}

void MovementInfo::SetForwardMovement(bool bMovement, bool bMoveForward)
{
   m_bForwardMovement = bMovement;
   m_bMoveForward = bMoveForward;
}

void MovementInfo::SetSidewaysMovement(bool bMovement, bool bMoveLeft)
{
   m_bSidewaysMovement = bMovement;
   m_bSidewaysMoveLeft = bMoveLeft;
}

Vector3d MovementInfo::PredictPosition(double dElapsedTime) const
{
   if (m_enMovementMode == movementStand)
      return m_vCurPos;

   if (m_enMovementMode == movementTarget)
   {
      // calculate direction
      Vector3d vDir = m_vDestPos - m_vCurPos;
      double dDist = vDir.Length();

      // check if distance is minimal
      if (vDir.Length() < 1e-3)
         return m_vDestPos; // reached destination position

      vDir.Normalize();

      // calculate distance so far
      vDir *= m_dSpeedInUnitsPerSec * dElapsedTime;

      if (vDir.Length() >= dDist)
         return m_vDestPos;

      return m_vCurPos + vDir;
   }

   if (m_enMovementMode == movementDirection)
   {
      Vector3d vDir(1.0, 0.0, 0.0);
      vDir.RotateY(m_dDirection);
      vDir.Normalize();

      // calculate distance so far
      vDir *= m_dSpeedInUnitsPerSec * dElapsedTime;

      return m_vCurPos + vDir;
   }

   if (m_enMovementMode == movementPlayer)
   {
      double m_dMoveSpeed = 0.0;
      if (m_bForwardMovement)
         m_dMoveSpeed = m_bMoveForward ? c_dMoveForwardSpeed : -c_dMoveBackwardSpeed;

      double m_dStrafeSpeed = 0.0;
      if (m_bSidewaysMovement)
         m_dStrafeSpeed = m_bSidewaysMoveLeft ? c_dStrafeSpeed : -c_dStrafeSpeed;

      Vector3d vNewPos = m_vCurPos;

      // calculate new position
      if (dElapsedTime > 0.0 && m_dMoveSpeed != 0.0 || m_dStrafeSpeed != 0.0)
      {
         Vector3d vDir(m_dStrafeSpeed, 0.0, m_dMoveSpeed);

         // normalize to larger speed count
         vDir.Normalize();
         vDir *= /*m_dSpeedInUnitsPerSec **/ std::max(fabs(m_dStrafeSpeed), fabs(m_dMoveSpeed));

         vDir.RotateY(m_dDirection);

         vDir *= dElapsedTime;
         vNewPos += vDir;
      }

      return vNewPos;
   }

   ATLASSERT(false);
   return m_vCurPos;
}

double MovementInfo::TimeToReachDestination() const
{
   switch (m_enMovementMode)
   {
   case movementStand:
      return 0.0;

   case movementTarget:
      {
         Vector3d vDir = m_vDestPos - m_vCurPos;
         return vDir.Length() / m_dSpeedInUnitsPerSec;
      }

   case movementDirection:
      return 1.0; // constant factor

   default:
      ATLASSERT(false);
   }

   return 0.0;
}

void MovementInfo::Serialize(ByteStream& stream) const
{
   ATLASSERT(m_enMovementMode <= 0xff);

   stream.Write8(BYTE(m_enMovementMode & 0xff));

   stream.WriteVector(m_vCurPos);
   stream.Write8(static_cast<unsigned char>(m_dSpeedInUnitsPerSec * 10.0));

   switch (m_enMovementMode)
   {
   case movementStand:
      break;

   case movementTarget:
      stream.WriteVector(m_vDestPos);
      break;

   case movementDirection:
      stream.Write16(static_cast<unsigned short>(m_dDirection));
      break;

   case movementPlayer:
      stream.Write16(static_cast<unsigned short>(m_dDirection));
      stream.Write8(
         (m_bForwardMovement  ? 0x01 : 0) |
         (m_bMoveForward      ? 0x02 : 0) |
         (m_bSidewaysMovement ? 0x04 : 0) |
         (m_bSidewaysMoveLeft ? 0x08 : 0));
      break;

   default:
      ATLASSERT(false);
   }
}

void MovementInfo::Deserialize(ByteStream& stream)
{
   m_enMovementMode = static_cast<T_enMovementMode>(stream.Read8());
   if (m_enMovementMode > movementMax)
      throw Exception(_T("invalid movement mode"), __FILE__, __LINE__);

   m_vCurPos = stream.ReadVector();
   m_dSpeedInUnitsPerSec = stream.Read8() * 0.1;

   switch (m_enMovementMode)
   {
   case movementStand:
      break;

   case movementTarget:
      m_vDestPos = stream.ReadVector();
      m_timerStartMovement.Restart();
      break;

   case movementDirection:
      m_dDirection = stream.Read16();
      if (m_dDirection >= 360.0)
         throw Exception(_T("invalid movement direction"), __FILE__, __LINE__);
      break;

   case movementPlayer:
      {
         m_dDirection = stream.Read16();
         if (m_dDirection >= 360.0)
            throw Exception(_T("invalid movement direction"), __FILE__, __LINE__);

         unsigned char ucFlags = stream.Read8();
         if ((ucFlags & 0x80) != 0)
            throw Exception(_T("invalid movement flags"), __FILE__, __LINE__);

         m_bForwardMovement  = (ucFlags & 0x01) != 0;
         m_bMoveForward      = (ucFlags & 0x02) != 0;
         m_bSidewaysMovement = (ucFlags & 0x04) != 0;
         m_bSidewaysMoveLeft = (ucFlags & 0x08) != 0;
      }
      break;

   default:
      ATLASSERT(false);
   }
}

bool MovementInfo::operator==(const MovementInfo& rhs) const throw()
{
   if (rhs.m_enMovementMode != m_enMovementMode)
      return false;

   if (m_enMovementMode == movementPlayer)
   {
      if (rhs.m_bForwardMovement != m_bForwardMovement ||
          rhs.m_bMoveForward != m_bMoveForward ||
          rhs.m_bSidewaysMovement != m_bSidewaysMovement ||
          rhs.m_bSidewaysMoveLeft != m_bSidewaysMoveLeft)
         return false;

      if (rhs.m_dDirection != m_dDirection)
         return false;
   }

   return true;
}
