//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file LocalController.hpp Local game controller class
//
#pragma once

// includes
#include "Network.hpp"
#include "IController.hpp"
#include "Object.hpp"

// forward references
class LocalModel;
class Command;
class MovementInfo;

/// \brief controller in a local game
/// \details duties of this class:
/// * converts movements, controls, keys, etc. to commands
/// * checks if command is valid/allowed
class NETWORK_DECLSPEC LocalController: public IController
{
public:
   /// ctor
   LocalController(LocalModel& localModel);

   // methods

   /// inits controller
   void Init(ObjectId playerId);

   // command shortcut functions

   /// selects a mobile (can be ObjectId::Null())
   void SelectMobile(ObjectId mobileId);

   /// returns player object id
   ObjectId PlayerId() const { return m_playerId; }

   // virtual methods from IController

   /// sends command
   virtual void SendCommand(Command& c) override;

   /// moves player
   virtual void MovePlayer(const MovementInfo& info) override;

protected:
   /// checks a command
   bool CheckCommand(const Command& c);

protected:
   /// model
   LocalModel& m_localModel;

private:
   /// player's object id
   ObjectId m_playerId;
};
