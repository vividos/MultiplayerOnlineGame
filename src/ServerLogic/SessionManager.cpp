//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SessionManager.cpp Session manager
//

// includes
#include "StdAfx.h"
#include "SessionManager.hpp"
#include "IAuthManager.hpp"
#include "ServerSessionImpl.hpp"

SessionManager::SessionManager(IAuthManager& authManager,
   IModel& worldModel, boost::asio::io_service& ioService)
:m_authManager(authManager),
 m_worldModel(worldModel),
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

std::shared_ptr<Session> SessionManager::CreateNewSession()
{
   std::shared_ptr<ServerSessionImpl> spSession(
      new ServerSessionImpl(m_ioService, m_worldModel, *this));

   // query auth manager for server authentication module to use (if any)
   std::shared_ptr<IServerAuthModule> spAuthModule = m_authManager.GetAuthenticationModule();
   if (spAuthModule != NULL)
      spSession->SetAuthenticationModule(spAuthModule);

   return spSession;
}

void SessionManager::ConnectSession(const ObjectId& /*objId*/, std::shared_ptr<Session> spSession)
{
   std::shared_ptr<ServerSessionImpl> spSessionImpl =
      std::dynamic_pointer_cast<ServerSessionImpl>(spSession);

   ATLASSERT(spSessionImpl != nullptr);

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
