//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IController.hpp MVC controller interface
//
#pragma once

// forward references
class Command;
class MovementInfo;

/// \brief the "controller" in the MVC pattern
class IController
{
public:
   /// dtor
   virtual ~IController() throw() {}

   /// sends command
   virtual void SendCommand(Command& c) = 0;

   /// moves player
   virtual void MovePlayer(const MovementInfo& info) = 0;
};
