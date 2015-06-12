//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AuthClientSession.hpp Client session with authentication
//
#pragma once

// includes
#include "Network.hpp"
#include "ClientSession.hpp"

// forward references
class IClientAuthModule;
class AuthInfo;

/// \brief client session with authentication
/// \details a session on the client side that supports authentication and encryption modules.
/// See also AuthServerSession for the counterpart class.
class NETWORK_DECLSPEC AuthClientSession: public ClientSession
{
public:
   DEFINE_INSTANCE(AuthClientSession)

   /// ctor
   AuthClientSession(boost::asio::io_service& ioService);
   /// dtor
   virtual ~AuthClientSession() throw() {}

   /// sets auth info
   void SetAuthInfo(const AuthInfo& info);

protected:
   /// message handler
   virtual bool OnReceiveMessage(RawMessage& msg) override;

   /// sets up session
   virtual void SetupSession();

private:
   /// called on successful connection
   virtual void OnConnectSuccess() override;

private:
   /// client authentication module (may be null)
   std::shared_ptr<IClientAuthModule> m_spAuthModule;

   /// indicates if setup of session is completed
   bool m_bSetupComplete;
};
