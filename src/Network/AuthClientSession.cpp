//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file AuthClientSession.cpp Client session with authentication
//

// includes
#include "stdafx.h"
#include "AuthClientSession.hpp"
#include "RawMessage.hpp"
#include "IClientAuthModule.hpp"
#include "SRPClientAuthModule.hpp"

AuthClientSession::AuthClientSession(boost::asio::io_service& ioService)
:ClientSession(ioService),
 m_bSetupComplete(false)
{
}

void AuthClientSession::SetAuthInfo(const AuthInfo& info)
{
   // set up auth module
   std::shared_ptr<IClientAuthModule> spClientAuthModule(new SRP::ClientAuthModule(info));

   m_spAuthModule = spClientAuthModule;
   m_spAuthModule->SetSession(shared_from_this());
}

bool AuthClientSession::OnReceiveMessage(RawMessage& msg)
{
   // check if we're authenticated yet
   if (m_spAuthModule != NULL && !m_spAuthModule->IsAuthenticated())
   {
      UpdateConnectState(connectStateAuthenticate);

      // check if it's a logout message
      if (msg.MessageId() == msgLogoutRequest)
      {
         UpdateConnectState(connectStateAuthFailed);
         Session::Close();
         return false;
      }

      try
      {
         if (m_spAuthModule->OnReceiveMessage(msg))
         {
            // authentication complete?
            if (m_spAuthModule->IsAuthenticated())
            {
               UpdateConnectState(connectStateLoggedIn);

               // check if auth module specifies encrypt and decrypt functions
               std::shared_ptr<IEncryptModule> spEncryptModule =
                  m_spAuthModule->GetEncryptModule();

               if (spEncryptModule != NULL)
                  SetEncryptModule(spEncryptModule);
               // set up session; may call derived class method
               SetupSession();
               m_bSetupComplete = true;
            }

            return true;
         }
      }
      catch(...)
      {
         UpdateConnectState(connectStateAuthFailed);
         throw; // rethrow
      }

      return false;
   }

   return ClientSession::OnReceiveMessage(msg);
}

void AuthClientSession::OnConnectSuccess()
{
   // when auth module is set, give it a chance for a message
   if (m_spAuthModule != NULL)
      m_spAuthModule->InitAuthentication();
   else
   {
      // no auth module
      if (!m_bSetupComplete)
      {
         SetupSession();
         m_bSetupComplete = true;
      }
   }
}

void AuthClientSession::SetupSession()
{
   // empty implementation; may be overwritten by derived class
}
