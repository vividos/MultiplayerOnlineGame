//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file SRPServerAuthModule.hpp SRP Server authentication module
//
#pragma once

// includes
#include "Base.hpp"
#include "IServerAuthModule.hpp"
#include <functional>

// forward references
namespace RC4
{
   class EncryptModule;
}

namespace SRP
{
class Server;

/// server authentication module for SRP auth
class ServerAuthModule: public IServerAuthModule
{
public:
   /// callback function to get auth info from server
   typedef std::function<void (const CString& cszUsername, std::vector<unsigned char>& vecPasswordKey,
      std::vector<unsigned char>& vecSalt, int& iAccountId)> T_fnGetServerAuthInfo;

   /// ctor
   ServerAuthModule(T_fnGetServerAuthInfo fnGetServerAuthInfo);
   /// dtor
   virtual ~ServerAuthModule() throw() {}

   /// returns account id
   int AccountId() const throw() { return m_iAccountId; }

   // virtual methods from IServerAuthModule

   virtual void InitAuthentication() override;
   virtual bool IsAuthenticated() const throw() override;
   virtual bool OnReceiveMessage(RawMessage& msg) override;
   virtual std::shared_ptr<IEncryptModule> GetEncryptModule() override;

private:
   /// called when auth request message is received
   void OnMessageAuthRequest(RawMessage& msg);
   /// called when verify client message is received
   void OnMessageVerifyClient(RawMessage& rawMessage);

private:
   /// callback function to get auth info
   T_fnGetServerAuthInfo m_fnGetServerAuthInfo;

   /// flag if client is already verified
   bool m_bIsVerifiedClient;

   /// SRP server object
   std::shared_ptr<SRP::Server> m_spServer;

   /// encrypt module
   std::shared_ptr<RC4::EncryptModule> m_spEncryptModule;

   /// account id from database;
   int m_iAccountId;
};

} // namespace SRP
