//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2012 Michael Fink
//
//! \file LocalModelSession.hpp Local session using LocalModel
//
#pragma once

// includes
#include "Network.hpp"
#include "AuthClientSession.hpp"

// forward references
class LocalModel;

/// \brief local session that updates LocalModel
class NETWORK_DECLSPEC LocalModelSession : public AuthClientSession
{
public:
   /// ctor
   LocalModelSession(LocalModel& model, boost::asio::io_service& ioService);

   /// dtor
   virtual ~LocalModelSession() throw() {}

private:
   // virtual methods from ClientSession

   /// message handler
   virtual bool OnReceiveMessage(RawMessage& msg) override;

   /// sets up session
   virtual void SetupSession() override;

   /// message handler
   void OnMessageAction(RawMessage& rawMsg);
   void OnMessageAddRemoveObject(RawMessage& rawMsg);
   void OnMessageUpdateObjectMovement(RawMessage& rawMsg);
   void OnMessageSessionInit(RawMessage& rawMsg);

private:
   /// local model
   LocalModel& m_model;
};
