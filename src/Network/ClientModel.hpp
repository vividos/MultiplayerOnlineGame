//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ClientModel.hpp Client model
//
#pragma once

// includes
#include "Network.hpp"
#include "LocalModel.hpp"
#include "IMessageSink.hpp"

/// model for client of a client/server app
class NETWORK_DECLSPEC ClientModel:
   public LocalModel,
   public IMessageSink
{
public:
   /// ctor
   ClientModel() throw() {}

   // virtual functions from LocalModel

   virtual void Tick(const TimeIndex& timeIndex) override;

   // virtual functions from IMessageSink

   virtual bool OnReceiveMessage(RawMessage& msg) override;

private:
   /// called when ActionMessage has been received
   void OnMessageAction(RawMessage& rawMsg);

   /// called when AddRemoveObjectMessage has been received
   void OnMessageAddRemoveObject(RawMessage& rawMsg);

   /// called when UpdateObjectMovementMessage has been received
   void OnMessageUpdateObjectMovement(RawMessage& rawMsg);

   /// called when SessionInitMessage has been received
   void OnMessageSessionInit(RawMessage& rawMsg);
};
