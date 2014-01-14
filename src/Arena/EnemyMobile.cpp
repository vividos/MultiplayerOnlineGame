//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file EnemyMobile.cpp Enemy mobile
//

// includes
#include "stdafx.h"
#include "EnemyMobile.hpp"
#include "LocalModel.hpp"

using Arena::EnemyMobile;

EnemyMobile::EnemyMobile(LocalModel& model, const Vector3d& vOffset)
:MobileActor(Uuid::New()),
 m_model(model),
 m_uiStage(0)
{
   m_vecPoints.push_back(Vector3d(10.0, 0.0, 10.0) + vOffset);
   m_vecPoints.push_back(Vector3d(50.0, 0.0, 10.0) + vOffset);
   m_vecPoints.push_back(Vector3d(50.0, 0.0, 50.0) + vOffset);
   m_vecPoints.push_back(Vector3d(10.0, 0.0, 50.0) + vOffset);

   Mobile::Pos(m_vecPoints[0]);
}

void EnemyMobile::Think(IModel& /*model*/, IActionQueue& /*actionQueue*/)
{
   if (m_uiStage == 0)
   {
      m_timer.Start();
      m_uiStage = 2;
      UpdateMovement();
   }

   // move to points in area
   if (m_timer.Elapsed() < 4.0)
      return;

   m_timer.Restart();
   m_uiStage++;
   if (m_uiStage == 5)
      m_uiStage = 1;

   UpdateMovement();
}

void EnemyMobile::UpdateMovement()
{
   // update movement info
   ATLASSERT(m_uiStage-1 < m_vecPoints.size());

   size_t uiNextPoint = m_uiStage-1;
   Vector3d vNextPos = m_vecPoints[uiNextPoint];

   size_t uiLastPoint = uiNextPoint == 0 ? 3 : uiNextPoint-1;
   Vector3d vCurrentPos = m_vecPoints[uiLastPoint];

   MovementInfo info(MovementInfo::movementTarget);
   info.Position(vCurrentPos);
   info.Destination(vNextPos);
   info.Speed(10.0);

   m_model.UpdateObjectMovement(Mobile::Id(), info);
}
