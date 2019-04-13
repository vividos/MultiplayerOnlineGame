//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ServerController.cpp Server side controller
//

// includes
#include "StdAfx.h"
#include "ServerController.hpp"
#include "ServerModel.hpp"
#include "RawMessage.hpp"
#include "CommandMessage.hpp"
#include "MovePlayerMessage.hpp"
#include "Mobile.hpp"

ServerController::ServerController(ServerModel& serverModel, IModel& worldModel)
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
   switch (msg.MessageId())
   {
   case msgCommand:
      OnMessageCommand(msg);
      break;

   case msgMovePlayer:
      OnMessageMovePlayer(msg);
      break;

   default:
      return false; // unknown message
   }

   return true;
}

void ServerController::OnMessageCommand(RawMessage& rawMsg)
{
   CommandMessage msg;
   ConstVectorRefStream stream(rawMsg.Data());
   msg.Deserialize(stream);

   Command& cmd = msg.GetCommand();

   // set actor id; isn't sent with message
   cmd.ActorId(m_serverModel.Player()->Id());

   // check command and execute it
   this->SendCommand(cmd);
}

void ServerController::OnMessageMovePlayer(RawMessage& rawMsg)
{
   MovePlayerMessage msg;
   ConstVectorRefStream stream(rawMsg.Data());
   msg.Deserialize(stream);

   this->MovePlayer(msg.Info());
}
