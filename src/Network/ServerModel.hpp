//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ServerModel.hpp Server side player model
//
#pragma once

// includes
#include "Network.hpp"
#include "IModel.hpp"
#include "ObjectMap.hpp"

// forward references
class Session;
class Message;

/// a player's "area of interest" on server side
class NETWORK_DECLSPEC ServerModel: public IModel
{
public:
   /// ctor
   ServerModel(IModel& worldModel)
      :m_worldModel(worldModel)
   {
   }

   // virtual methods from IModel

   virtual void InitialUpdate(MobilePtr spPlayer) override;

   virtual void Tick(const TimeIndex& timeIndex) override;

   virtual void ReceiveAction(ActionPtr spAction) override;

   virtual void ReceiveCommand(Command& c) override;

   virtual void AddRemoveObject(const std::vector<ObjectPtr>& vecObjectsToAdd,
      const std::vector<ObjectId>& vecObjectsToRemove) override;

   virtual void UpdateObjectMovement(const ObjectId& id, const MovementInfo& info) override;

   // methods

   /// sets session object
   void SetSession(std::shared_ptr<Session> spSession){ m_spSession = spSession; }

   /// updates player movement
   void MovePlayer(const MovementInfo& info);

   /// returns player mobile
   MobilePtr Player(){ return m_spPlayer; }

private:
   /// sends message to client
   void SendMessage(const Message& msg);

   /// queues action for processing
   void QueueAction(ActionPtr spAction);

private:
   /// world model
   IModel& m_worldModel;

   /// session to send messages to client
   std::shared_ptr<Session> m_spSession;

   /// player mobile object
   MobilePtr m_spPlayer;

   /// object map with objects only relevant to client
   ObjectMap m_objectMap;
};
