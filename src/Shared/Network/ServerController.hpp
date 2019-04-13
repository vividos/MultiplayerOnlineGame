//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ServerController.hpp Server side controller
//
#pragma once

// includes
#include "Network.hpp"
#include "IController.hpp"
#include "IMessageSink.hpp"
#include "Object.hpp"

// forward references
class ServerModel;
class IModel;

class NETWORK_DECLSPEC ServerController:
   public IController,
   public IMessageSink
{
public:
   /// ctor
   ServerController(ServerModel& serverModel, IModel& worldModel);
   /// dtor
   virtual ~ServerController() {}

   /// inits controller
   void Init(ObjectId playerId);

   // virtual methods from IController

   /// sends command
   virtual void SendCommand(Command& c) override;

   /// moves player
   virtual void MovePlayer(const MovementInfo& info) override;

   // virtual methods from IMessageSink

   /// called when new message has arrived; returns true when message has been handled
   virtual bool OnReceiveMessage(RawMessage& msg) override;

private:
   /// called for received CommandMessage
   void OnMessageCommand(RawMessage& rawMsg);

   /// called for received MovePlayerMessage
   void OnMessageMovePlayer(RawMessage& rawMsg);

private:
   /// player object id
   ObjectId m_playerId;

   /// server player model
   ServerModel& m_serverModel;

   /// world model
   IModel& m_worldModel;
};
