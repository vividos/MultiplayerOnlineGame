#pragma once

// includes
#include "Base.hpp"
#include "IClientAuthModule.hpp"
#include "AuthInfo.hpp"

// forward references
namespace RC4
{
   class EncryptModule;
}

namespace SRP
{
class Client;

/// client authentication module for SRP auth
class ClientAuthModule: public IClientAuthModule
{
public:
   /// ctor
   ClientAuthModule(const AuthInfo& authInfo);
   /// dtor
   virtual ~ClientAuthModule() throw() {}

   virtual void InitAuthentication() override;
   virtual bool IsAuthenticated() const throw() override;
   virtual bool OnReceiveMessage(RawMessage& msg) override;
   virtual std::shared_ptr<IEncryptModule> GetEncryptModule() override;

private:
   /// called when auth response message is received
   void OnMessageAuthResponse(RawMessage& rawMessage);
   /// called when verify server message is received
   void OnMessageVerifyServer(RawMessage& rawMessage);

private:
   /// auth info to send
   AuthInfo m_authInfo;

   /// flag if server is already verified
   bool m_bIsVerifiedServer;

   /// SRP client object
   std::shared_ptr<SRP::Client> m_spClient;

   /// encrypt module
   std::shared_ptr<RC4::EncryptModule> m_spEncryptModule;
};

} // namespace SRP
