//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AuthServerSession.hpp Server session with authentication
//
#pragma once

// includes
#include "Network.hpp"
#include "ServerSession.hpp"

// forward references
class IServerAuthModule;

/// \brief server session that supports authentication
/// \details Supports authentication modules.
/// See also AuthClientSession for the counterpart class.
class NETWORK_DECLSPEC AuthServerSession: public ServerSession
{
public:
   /// ctor
   AuthServerSession(boost::asio::io_service& ioService)
      :ServerSession(ioService)
   {
   }
   /// dtor
   virtual ~AuthServerSession() {}

   /// sets auth module to use when a client tries to log in
   void SetAuthenticationModule(std::shared_ptr<IServerAuthModule> spAuthModule);

   /// starts server session
   virtual void Start() override;

protected:
   /// called when message is received for this session
   virtual bool OnReceiveMessage(RawMessage& msg) override;

   /// called when session is connected and ready for set up
   virtual void SetupSession();

   /// returns account id from server auth module
   int GetAccountId();

private:
   /// server auth module
   std::shared_ptr<IServerAuthModule> m_spAuthModule;
};
