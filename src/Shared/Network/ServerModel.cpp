//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ServerModel.cpp Server side player model
//

// includes
#include "StdAfx.h"
#include "ServerModel.hpp"
#include "ServerSession.hpp"
#include "SessionInitMessage.hpp"
#include "ActionMessage.hpp"
#include "AddRemoveObjectMessage.hpp"
#include "UpdateObjectMovementMessage.hpp"
#include "Mobile.hpp"
#include "Action.hpp"

void ServerModel::InitialUpdate(MobilePtr spPlayer)
{
   m_spPlayer = spPlayer;

   // send init message
   {
      SessionInitMessage msg(spPlayer);
      SendMessage(msg);
   }

   // send initial list of objects
   {
      ObjectId playerId = spPlayer->Id();

      ObjectMap::T_mapObjects objMap = m_objectMap.GetMap();

      std::vector<ObjectPtr> vecObjectsToAdd;
      ObjectMap::T_mapObjects::const_iterator iter = objMap.begin(), stop = objMap.end();
      for (; iter != stop; ++iter)
      {
         if (iter->first != playerId)
            vecObjectsToAdd.push_back(iter->second);
      }

      if (!vecObjectsToAdd.empty())
      {
         std::vector<ObjectId> vecObjectsToRemove;
         AddRemoveObjectMessage msg(vecObjectsToAdd, vecObjectsToRemove);
         SendMessage(msg);
      }
   }
}

void ServerModel::Tick(const TimeIndex& /*timeIndex*/)
{
   // no tick processing; the world model does this for us
   ATLASSERT(false);
}

void ServerModel::ReceiveAction(ActionPtr spAction)
{
   // apply to this model
   QueueAction(spAction);

   // send to client
   ActionMessage msg(spAction);
   SendMessage(msg);
}

void ServerModel::ReceiveCommand(Command& c)
{
   // hand over to world model
   m_worldModel.ReceiveCommand(c);
}

void ServerModel::AddRemoveObject(const std::vector<ObjectPtr>& vecObjectsToAdd,
   const std::vector<ObjectId>& vecObjectsToRemove)
{
   // apply to this model
   {
      ObjectMap& objMap = m_objectMap;

      for (size_t i=0,iMax=vecObjectsToRemove.size(); i<iMax; i++)
         objMap.RemoveObject(vecObjectsToRemove[i]);

      for (size_t i=0,iMax=vecObjectsToAdd.size(); i<iMax; i++)
         objMap.AddObject(vecObjectsToAdd[i]);
   }

   // send to client
   AddRemoveObjectMessage msg(vecObjectsToAdd, vecObjectsToRemove);
   SendMessage(msg);
}

void ServerModel::UpdateObjectMovement(const ObjectId& id, const MovementInfo& info)
{
   // apply to this model
   {
      // TODO
   }

   // send to client
   UpdateObjectMovementMessage msg(id, info);
   SendMessage(msg);
}

void ServerModel::MovePlayer(const MovementInfo& info)
{
   Player()->UpdateMovementInfo(info);

   // send to world model
   m_worldModel.UpdateObjectMovement(Player()->Id(), info);
}

void ServerModel::SendMessage(const Message& msg)
{
   ATLASSERT(m_spSession != NULL);

   // lock session, if possible
   try
   {
      m_spSession->SendMessage(msg);
   }
   catch(const Exception& /*ex*/)
   {
      //LOG_ERROR(_T("session vanished while sending a player instance message"), Log::Server::World::Player);

      // TODO
      // session was destroyed; remove from player instance map

      throw;
   }
}

void ServerModel::QueueAction(ActionPtr spAction)
{
   // resolve argument
   ObjectRef& ref = spAction->ArgumentRef();
   if (ref.m_sp == NULL)
      ref.m_sp = m_objectMap.FindObject(ref.m_id).m_sp;

   spAction->Do(*this);
}
