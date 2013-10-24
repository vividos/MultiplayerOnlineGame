//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file SessionManager.cpp Session manager
//

// includes
#include "StdAfx.h"
#include "SessionManager.hpp"
#include "AuthServerSession.hpp"
#include "IAuthManager.hpp"

SessionManager::SessionManager(IAuthManager& authManager, boost::asio::io_service& ioService)
:m_authManager(authManager),
 m_ioService(ioService)
{
}

void SessionManager::Add(std::weak_ptr<ServerSession> wpClient)
{
   RecursiveMutex::LockType lock(m_mtxAllClients);

   m_setAllClients.insert(wpClient);
}

void SessionManager::Remove(std::weak_ptr<ServerSession> wpClient)
{
   RecursiveMutex::LockType lock(m_mtxAllClients);

   m_setAllClients.erase(wpClient);
}

void SessionManager::Cleanup()
{
   RecursiveMutex::LockType lock(m_mtxAllClients);

   T_setAllClients::iterator iter = m_setAllClients.begin(), stop = m_setAllClients.end();
   for (; iter != stop;)
   {
      // check if we can still lock the weak ptr
      const std::weak_ptr<ServerSession>& wpClient = *iter;
      std::shared_ptr<ServerSession> spClient(wpClient.lock());
      if (spClient == NULL)
      {
         T_setAllClients::iterator removeIter = iter;
         ++iter;
         m_setAllClients.erase(removeIter);
         LOG_WARN(_T("SessionManager removed stale session"), Log::Server::Session);
      }
      else
         ++iter;
   }
}

void SessionManager::LogoutAll()
{
   RecursiveMutex::LockType lock(m_mtxAllClients);

   T_setAllClients::iterator iter = m_setAllClients.begin(), stop = m_setAllClients.end();
   for (; iter != stop; ++iter)
   {
      const std::weak_ptr<ServerSession>& wpClient = *iter;
      std::shared_ptr<ServerSession> spClient(wpClient.lock());
      if (spClient != NULL)
      {
         spClient->Logout(LogoutMessage::logoutServerShutdown);
         spClient->Close();
      }
   }
}

class SessionManagerServerSession: public AuthServerSession
{
public:
   SessionManagerServerSession(boost::asio::io_service& ioService, SessionManager& sessionManager)
      :AuthServerSession(ioService),
       m_sessionManager(sessionManager)
   {
   }
   virtual ~SessionManagerServerSession() throw() {}

private:
   virtual void SetupSession() override;

   virtual void OnConnectionClosing() override;

private:
   SessionManager& m_sessionManager;
};

void SessionManagerServerSession::SetupSession()
{
   //m_serverSideModel.SetSession(Session::shared_from_this());

   // register in session manager
   std::weak_ptr<ServerSession> wpSession =
      std::dynamic_pointer_cast<ServerSession>(shared_from_this());

   m_sessionManager.Add(wpSession);

   // load from database
   //ObjectPtr spPlayer = m_playerInstanceManager.AddPlayer(GetAccountId(), wpSession);

   //m_controller.Init(spPlayer->Id());

   //MobilePtr spMobile = std::dynamic_pointer_cast<Mobile>(spPlayer);
   //m_serverSideModel.InitialUpdate(spMobile);
}

void SessionManagerServerSession::OnConnectionClosing()
{
   // unregister in session manager
   std::weak_ptr<ServerSession> wpSession =
      std::dynamic_pointer_cast<ServerSession>(shared_from_this());

   m_sessionManager.Remove(wpSession);
}

std::shared_ptr<Session> SessionManager::CreateNewSession()
{
   std::shared_ptr<SessionManagerServerSession> spSession(
      new SessionManagerServerSession(m_ioService, *this));
   // TODO
   //   new ServerSession(m_worldModel, m_playerInstanceManager, m_ioService));

   // query auth manager for server authentication module to use (if any)
   std::shared_ptr<IServerAuthModule> spAuthModule = m_authManager.GetAuthenticationModule();
   if (spAuthModule != NULL)
      spSession->SetAuthenticationModule(spAuthModule);

   return spSession;
}

void SessionManager::ConnectSession(const ObjectId& /*objId*/, std::shared_ptr<Session> /*spSession*/)
{
   // TODO connect player id with session
}

std::weak_ptr<Session> SessionManager::FindSession(const ObjectId& /*objId*/)
{
   // TODO implement
   return std::weak_ptr<Session>();
}

void SessionManager::InitSession(std::shared_ptr<Session> /*spSession*/)
{
   // TODO implement
}
