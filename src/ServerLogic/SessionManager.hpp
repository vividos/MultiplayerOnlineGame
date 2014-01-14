//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SessionManager.hpp Session manager
//
#pragma once

// includes
#include "ServerLogic.hpp"
#include <ulib/RecursiveMutex.hpp>
#include "ISessionManager.hpp"
#include <set>

// forward references
class IAuthManager;

/// \brief session manager
/// \details manages all sessions connected to the server
class SERVERLOGIC_DECLSPEC SessionManager: public ISessionManager
{
public:
   DEFINE_INSTANCE(SessionManager)

   /// ctor
   SessionManager(IAuthManager& authManager, boost::asio::io_service& ioService);
   /// dtor
   virtual ~SessionManager() throw() {}

   /// inits a new session
   void InitSession(std::shared_ptr<Session> spSession);

   /// cleans up all clients that are not existant anymore
   void Cleanup();

   /// logs out all connected clients
   void LogoutAll();

   /// adds a server session to manager
   void Add(std::weak_ptr<ServerSession> wpClient);

   /// removes a server session from manager
   void Remove(std::weak_ptr<ServerSession> wpClient);

private:
   // virtual methods from ISessionManager

   virtual std::shared_ptr<Session> CreateNewSession() override;
   virtual void ConnectSession(const ObjectId& objId, std::shared_ptr<Session> spSession) override;
   virtual std::weak_ptr<Session> FindSession(const ObjectId& objId) override;

private:
   /// authentication manager
   IAuthManager& m_authManager;

   /// io service
   boost::asio::io_service& m_ioService;

   /// type of set to store server sessions
   typedef std::set<std::weak_ptr<ServerSession>, std::owner_less<std::weak_ptr<ServerSession>>> T_setAllClients;

   /// set to store server sessions
   T_setAllClients m_setAllClients;

   /// mutex to protect m_setAllClients
   RecursiveMutex m_mtxAllClients;
};
