//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ClientModel.cpp Client model
//

// includes
#include "stdafx.h"
#include "ClientModel.hpp"
#include "RawMessage.hpp"
#include "ByteStream.hpp"
#include "ActionMessage.hpp"
#include "AddRemoveObjectMessage.hpp"
#include "ActionMessage.hpp"
#include "UpdateObjectMovementMessage.hpp"
#include "SessionInitMessage.hpp"

void ClientModel::Tick(const TimeIndex&)
{
   // no tick processing; the server does this for us
}

bool ClientModel::OnReceiveMessage(RawMessage& msg)
{
   switch (msg.MessageId())
   {
   case msgAction:
      OnMessageAction(msg);
      break;

   case msgAddRemoveObject:
      OnMessageAddRemoveObject(msg);
      break;

   case msgUpdateObjectMovement:
      OnMessageUpdateObjectMovement(msg);
      break;

   case msgSessionInit:
      OnMessageSessionInit(msg);
      break;

   default:
      return false; // still not handled
   }

   return true;
}

void ClientModel::OnMessageAction(RawMessage& rawMsg)
{
   ActionMessage msg;
   ConstVectorRefStream stream(rawMsg.Data());
   msg.Deserialize(stream);

   // apply action using local model
   this->ReceiveAction(msg.GetAction());
}

void ClientModel::OnMessageAddRemoveObject(RawMessage& rawMsg)
{
   AddRemoveObjectMessage msg;
   ConstVectorRefStream stream(rawMsg.Data());
   msg.Deserialize(stream);

   // apply changes using local model
   this->AddRemoveObject(msg.GetObjectsToAdd(), msg.GetObjectsToRemove());
}

void ClientModel::OnMessageUpdateObjectMovement(RawMessage& rawMsg)
{
   UpdateObjectMovementMessage msg;
   ConstVectorRefStream stream(rawMsg.Data());
   msg.Deserialize(stream);

   this->UpdateObjectMovement(msg.Id(), msg.Info());
}

void ClientModel::OnMessageSessionInit(RawMessage& rawMsg)
{
   SessionInitMessage msg;
   ConstVectorRefStream stream(rawMsg.Data());
   msg.Deserialize(stream);

   // apply action using local model
   this->InitialUpdate(msg.Player());
}
