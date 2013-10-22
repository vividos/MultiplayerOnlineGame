//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ServerController.cpp Server side controller
//

// includes
#include "StdAfx.h"
#include "ServerController.hpp"
#include "ServerModel.hpp"

ServerController::ServerController(ServerModel& serverModel, IModel& worldModel) throw()
:m_playerId(ObjectId::Null()),
 m_serverModel(serverModel),
 m_worldModel(worldModel)
{
}

void ServerController::Init(ObjectId playerId)
{
   m_playerId = playerId;
}

void ServerController::SendCommand(Command& c)
{
   // TODO check command

   // send to world model
   m_worldModel.ReceiveCommand(c);
}

void ServerController::MovePlayer(const MovementInfo& info)
{
   // TODO check movement

   // send to server side model
   m_serverModel.MovePlayer(info);

   // send to world model
   m_worldModel.UpdateObjectMovement(m_playerId, info);
}

bool ServerController::OnReceiveMessage(RawMessage& msg)
{
   // TODO implement
   return false;
}
