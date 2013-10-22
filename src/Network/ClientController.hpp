//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ClientController.hpp Client side controller class
//
#pragma once

// includes
#include "Network.hpp"
#include "LocalController.hpp"

// forward references
class ClientSession;
class ClientModel;

/// \brief client controller in a distributed game
/// \details duties of this class:
/// * forwards commands to a client session
/// * forwards commands to a local ClientModel, as hint
class NETWORK_DECLSPEC ClientController: public LocalController
{
public:
   /// ctor
   ClientController(ClientSession& session, ClientModel& model);

private:
   // virtual methods from LocalController

   virtual void SendCommand(Command& c) override;

   virtual void MovePlayer(const MovementInfo& info) override;

private:
   /// client session
   ClientSession& m_session;

   /// client model
   ClientModel& m_clientModel;
};
