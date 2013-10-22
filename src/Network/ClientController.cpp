//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ClientController.cpp ClientController class
//

// includes
#include "StdAfx.h"
#include "ClientController.hpp"
#include "LocalModel.hpp"
#include "ClientModel.hpp"
#include "Mobile.hpp"
#include "ClientSession.hpp"
#include "CommandMessage.hpp"
#include "MovePlayerMessage.hpp"

ClientController::ClientController(ClientSession& session, ClientModel& model)
:LocalController(model),
 m_session(session),
 m_clientModel(model)
{
}

void ClientController::SendCommand(Command& c)
{
   if (!LocalController::CheckCommand(c))
      return;

   // send to model, as hint
   m_localModel.ReceiveCommand(c);

   // send to session
   CommandMessage msg(c);
   m_session.SendMessage(msg);
}

void ClientController::MovePlayer(const MovementInfo& info)
{
   // apply to player in local model
   m_localModel.Player()->UpdateMovementInfo(info);

   // also send to session
   MovePlayerMessage msg(info);
   m_session.SendMessage(msg);
}
