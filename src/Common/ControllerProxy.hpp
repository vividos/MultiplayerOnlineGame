//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ControllerProxy.hpp Controller proxy class
//
#pragma once

// includes
#include "IController.hpp"
#include "ISession.hpp"
#include "CommandMessage.hpp"
#include "MovePlayerMessage.hpp"

/// \brief MVC "controller" proxy for Session
/// \details mix-in class to implement IController interface for a Session derived class
/// \tparam TSession a ISession derived class
template <typename TSession>
class ControllerProxy: public IController
{
public:
   /// dtor
   virtual ~ControllerProxy() throw() {}

   /// sends command using session
   virtual void SendCommand(Command& cmd) override
   {
      TSession* pSession = static_cast<TSession*>(this);
      ISession& rSession = *pSession; // only works when TSession is derived from ISession

      CommandMessage msg(cmd);
      rSession.SendMessage(msg);
   }

   /// sends player movement using session
   virtual void MovePlayer(const MovementInfo& info) override
   {
      TSession* pSession = static_cast<TSession*>(this);
      ISession& rSession = *pSession; // only works when TSession is derived from ISession

      MovePlayerMessage msg(info);
      rSession.SendMessage(msg);
   }
};
