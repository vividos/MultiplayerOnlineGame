//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2012 Michael Fink
//
//! \file LocalModelSession.cpp Local session using LocalModel
//

// includes
#include "StdAfx.h"
#include "LocalModelSession.hpp"
#include "RawMessage.hpp"
#include "ActionMessage.hpp"
#include "AddRemoveObjectMessage.hpp"
#include "UpdateObjectMovementMessage.hpp"
#include "SessionInitMessage.hpp"
#include "LocalModel.hpp"

LocalModelSession::LocalModelSession(LocalModel& model, boost::asio::io_service& ioService)
:AuthClientSession(ioService),
 m_model(model)
{
}

bool LocalModelSession::OnReceiveMessage(RawMessage& msg)
{
   if (AuthClientSession::OnReceiveMessage(msg))
      return true;

   // not handled by parent
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

void LocalModelSession::SetupSession()
{
   // TODO init LocalModel
}

void LocalModelSession::OnMessageAction(RawMessage& rawMsg)
{
   ActionMessage msg;
   ConstVectorRefStream stream(rawMsg.Data());
   msg.Deserialize(stream);

   // apply action using local model
   m_model.ReceiveAction(msg.GetAction());
}

void LocalModelSession::OnMessageAddRemoveObject(RawMessage& rawMsg)
{
   AddRemoveObjectMessage msg;
   ConstVectorRefStream stream(rawMsg.Data());
   msg.Deserialize(stream);

   // apply changes using local model
   m_model.AddRemoveObject(msg.GetObjectsToAdd(), msg.GetObjectsToRemove());
}

void LocalModelSession::OnMessageUpdateObjectMovement(RawMessage& rawMsg)
{
   UpdateObjectMovementMessage msg;
   ConstVectorRefStream stream(rawMsg.Data());
   msg.Deserialize(stream);

   m_model.UpdateObjectMovement(msg.Id(), msg.Info());
}

void LocalModelSession::OnMessageSessionInit(RawMessage& rawMsg)
{
   SessionInitMessage msg;
   ConstVectorRefStream stream(rawMsg.Data());
   msg.Deserialize(stream);

   // apply action using local model
   m_model.InitialUpdate(msg.Player());

   UpdateConnectState(connectStateInitialized);
}
