//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Mobile.cpp Mobile class
//

// includes
#include "StdAfx.h"
#include "Mobile.hpp"
#include "ByteStream.hpp"

void Mobile::UpdateMovementInfo(const MovementInfo& info)
{
   m_movementInfo = info;

   Pos(info.Position());
   ViewAngle(unsigned(info.ViewAngle()));

   m_timerMovement.Restart();
}

void Mobile::Move()
{
   double dElapsed = m_timerMovement.Elapsed();

   Vector3d vPos = m_movementInfo.PredictPosition(dElapsed);
   Pos(vPos);
}

void Mobile::Serialize(ByteStream& stream) const
{
   Object::Serialize(stream);

   stream.Write16(m_displayInfo.Info());

   stream.Write16(static_cast<unsigned short>(m_uiMovementAngle)); //0..360
   // view angle isn't serialized
   stream.Write32(m_uiHealthPoints);
}

void Mobile::Deserialize(ByteStream& stream)
{
   Object::Deserialize(stream);

   m_displayInfo = MobileDisplayInfo(stream.Read16());
   if (!m_displayInfo.IsValid())
      throw Exception(_T("invalid Mobile display info"), __FILE__, __LINE__);

   m_uiMovementAngle = stream.Read16();
   if (m_uiMovementAngle > 359)
      throw Exception(_T("invalid Mobile movement angle"), __FILE__, __LINE__);

   // view angle isn't sent, so set it equal to movement angle
   m_uiViewAngle = m_uiMovementAngle;

   m_uiHealthPoints = stream.Read32();
}
