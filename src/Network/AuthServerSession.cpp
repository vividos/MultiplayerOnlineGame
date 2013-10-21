//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file AuthServerSession.cpp Server session with authentication
//

// includes
#include "stdafx.h"
#include "AuthServerSession.hpp"
#include "RawMessage.hpp"
#include "IServerAuthModule.hpp"
#include "AuthException.hpp"
#include "SRPServerAuthModule.hpp"

//
// AuthServerSession
//

void AuthServerSession::SetAuthenticationModule(std::shared_ptr<IServerAuthModule> spAuthModule)
{
   m_spAuthModule = spAuthModule;
   m_spAuthModule->SetSession(shared_from_this());
}

void AuthServerSession::Start()
{
   // give authentication first chance of messaging
   if (m_spAuthModule)
      m_spAuthModule->InitAuthentication();
   else
   {
      SetupSession();
   }

   Session::Start();
}

bool AuthServerSession::OnReceiveMessage(RawMessage& msg)
{
   // check if we're authenticated yet
   if (m_spAuthModule != NULL && !m_spAuthModule->IsAuthenticated())
   {
      try
      {
         if (m_spAuthModule->OnReceiveMessage(msg))
         {
            // authentication complete?
            if (m_spAuthModule->IsAuthenticated())
            {
               // check if auth module specifies encrypt module
               std::shared_ptr<IEncryptModule> spEncryptModule =
                  m_spAuthModule->GetEncryptModule();

               if (spEncryptModule != NULL)
                  SetEncryptModule(spEncryptModule);

               // set up session; may call derived class method
               SetupSession();
            }

            return true;
         }
      }
      catch(const AuthException& /*ex*/)
      {
         Logout(LogoutMessage::logoutUserPassUnknown);
         Close();

         throw;
      }
      catch(...)
      {
         Logout(LogoutMessage::logoutOther);
         Close();

         throw;
      }
   }

   return ServerSession::OnReceiveMessage(msg);
}

void AuthServerSession::SetupSession()
{
}

int AuthServerSession::GetAccountId()
{
   if (m_spAuthModule == NULL)
      return 0;

   std::shared_ptr<SRP::ServerAuthModule> spAuthModule =
      std::dynamic_pointer_cast<SRP::ServerAuthModule>(m_spAuthModule);
   if (spAuthModule == NULL)
      return 0;

   return spAuthModule->AccountId();
}
