//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AuthModuleTester.hpp Authentication module tester class
//
#pragma once

// includes
#include "ISession.hpp"
#include "ByteStream.hpp"
#include "Message.hpp"
#include "RawMessage.hpp"
#include "IClientAuthModule.hpp"
#include "IServerAuthModule.hpp"
#include "IEncryptModule.hpp"
#include <functional>
#include <memory>

class TestSession:
   public ISession,
   public std::enable_shared_from_this<TestSession>
{
public:
   /// dtor
   virtual ~TestSession() throw() {}

private:
   virtual void SendMessage(const Message& msg) override
   {
      VectorStream inStream;
      msg.Serialize(inStream);

      // deserialize again
      RawMessage rawMessage(msg.MessageId(), inStream.Data());

      // send
      bool bRet = m_fnSendMessage(rawMessage);
      if (!bRet)
         throw std::runtime_error("TestSession: packet wasn't handled"); // must handle all packets
   }

protected:
   std::function<bool (RawMessage& msg)> m_fnSendMessage;
};

class TestClientSession: public TestSession
{
public:
   void RegisterServerModule(IServerAuthModule& serverAuthModule, IClientAuthModule& clientAuthModule)
   {
      m_fnSendMessage = std::bind(
         &IServerAuthModule::OnReceiveMessage, &serverAuthModule, std::placeholders::_1);

      clientAuthModule.SetSession(shared_from_this());
   }
};

class TestServerSession: public TestSession
{
public:
   void RegisterClientModule(IClientAuthModule& clientAuthModule, IServerAuthModule& serverAuthModule)
   {
      m_fnSendMessage = std::bind(
         &IClientAuthModule::OnReceiveMessage, &clientAuthModule, std::placeholders::_1);

      serverAuthModule.SetSession(shared_from_this());
   }
};

namespace UnitTest
{

struct AuthModuleTester
{
   AuthModuleTester(IServerAuthModule& serverAuthModule, IClientAuthModule& clientAuthModule)
      :m_serverAuthModule(serverAuthModule),
       m_clientAuthModule(clientAuthModule),
       m_spServerSession(new TestServerSession),
       m_spClientSession(new TestClientSession)
   {
   }

   bool Test()
   {
      try
      {
         // set up sessions
         m_spServerSession->RegisterClientModule(m_clientAuthModule, m_serverAuthModule);
         m_spClientSession->RegisterServerModule(m_serverAuthModule, m_clientAuthModule);

         // first, give server a chance for a request
         m_serverAuthModule.InitAuthentication();

         // next, give client a chance
         m_clientAuthModule.InitAuthentication();

         // after shaking hands, modules must both be authenticated
         if (!m_serverAuthModule.IsAuthenticated())
            return false;

         if (!m_clientAuthModule.IsAuthenticated())
            return false;

         // check encryption modules
         std::shared_ptr<IEncryptModule> spServerEncryption = m_serverAuthModule.GetEncryptModule();
         std::shared_ptr<IEncryptModule> spClientEncryption = m_clientAuthModule.GetEncryptModule();

         if ((spServerEncryption == NULL) != (spClientEncryption == NULL))
            return false; // either none or both must have a module

         if (spServerEncryption != NULL)
         {
            // encrypt data, and decrypt again
            std::vector<unsigned char> vecData;
            vecData.push_back(42);
            vecData.push_back(64);

            spServerEncryption->EncryptWrite(vecData.begin(), vecData.end());
            spClientEncryption->DecryptRead(vecData.begin(), vecData.end());

            if (vecData[0] != 42 && vecData[0] != 64)
               return false;

            spClientEncryption->EncryptWrite(vecData.begin(), vecData.end());
            spServerEncryption->DecryptRead(vecData.begin(), vecData.end());

            if (vecData[0] != 42 && vecData[0] != 64)
               return false;
         }
      }
      catch(...)
      {
         return false;
      }

      return true;
   }

   IServerAuthModule& m_serverAuthModule;
   IClientAuthModule& m_clientAuthModule;

   std::shared_ptr<TestServerSession> m_spServerSession;
   std::shared_ptr<TestClientSession> m_spClientSession;
};

} // namespace UnitTest
