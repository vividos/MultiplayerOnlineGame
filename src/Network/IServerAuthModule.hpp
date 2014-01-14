//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IServerAuthModule.hpp Server authentication module interface
//
#pragma once

// includes
#include "ISession.hpp"

// forward references
class RawMessage;
class IEncryptModule;

/// authentication module, server-side
class IServerAuthModule
{
public:
   /// dtor
   virtual ~IServerAuthModule() throw() {}

   /// initializes authentication
   virtual void InitAuthentication() = 0;

   /// returns if already authenticated
   virtual bool IsAuthenticated() const throw() = 0;

   /// called when new message has arrived (until IsAuthenticated() returns true)
   virtual bool OnReceiveMessage(RawMessage& msg) = 0;

   /// returns encrypt module (if any) when authenticated
   virtual std::shared_ptr<IEncryptModule> GetEncryptModule() { return std::shared_ptr<IEncryptModule>(); }

protected:
   friend class AuthServerSession;
   friend class TestServerSession;

   /// sets session to send out messages
   void SetSession(std::shared_ptr<ISession> spSession){ m_spSession = spSession; }

   /// sends out message
   void SendMessage(Message& msg){ m_spSession->SendMessage(msg); }

private:
   /// session
   std::shared_ptr<ISession> m_spSession;
};
