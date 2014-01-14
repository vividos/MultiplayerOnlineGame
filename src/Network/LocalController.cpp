//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file LocalController.cpp Local game controller class
//

// includes
#include "StdAfx.h"
#include "LocalController.hpp"
#include "LocalModel.hpp"
#include "Command.hpp"
#include "Mobile.hpp"

LocalController::LocalController(LocalModel& localModel)
:m_localModel(localModel),
 m_playerId(ObjectId::Null()) // is set in Init()
{
}

void LocalController::Init(ObjectId playerId)
{
   m_playerId = playerId;
}

void LocalController::SelectMobile(ObjectId mobileId)
{
   Command c(cmdSelectObject, m_playerId, mobileId);
   SendCommand(c);
}

void LocalController::SendCommand(Command& c)
{
   if (!CheckCommand(c))
      return;

   // send to model
   m_localModel.ReceiveCommand(c);
}

bool LocalController::CheckCommand(const Command& c)
{
   if (c.Id() == cmdHitMobile)
   {
      // check if mobile is selected
      if (c.ArgumentId() == ObjectId::Null())
         return false; // can't hit
   }
   else
   if (c.Id() == cmdSelectObject)
   {
      if (c.ArgumentId() == ObjectId::Null())
         return true; // unselect always possible

      // check if object is in object map
      const ObjectMap& objMap = m_localModel.GetObjectMap();

      ObjectId id = c.ArgumentId();
      if (!objMap.IsInMap(id))
         return false;

      // check if object is in range
      ObjectRef objRef = objMap.FindObject(id);

      Vector3d vPos = objRef.m_sp->Pos();
      vPos -= m_localModel.Player()->Pos();

      if (vPos.Length() > c_dMaxSelectionDistance)
         return false;
   }

   return true;
}

void LocalController::MovePlayer(const MovementInfo& info)
{
   // apply to player in client model
   m_localModel.Player()->UpdateMovementInfo(info);
}
