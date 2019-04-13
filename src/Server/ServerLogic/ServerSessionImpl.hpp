#pragma once

// includes
#include "AuthServerSession.hpp"
#include "ServerModel.hpp"

// forward references
class SessionManager;
class IModel;

/// server session implementation with remote model
class ServerSessionImpl : public AuthServerSession
{
public:
   /// ctor
   ServerSessionImpl(boost::asio::io_service& ioService, IModel& worldModel, SessionManager& sessionManager);
   /// dtor
   virtual ~ServerSessionImpl() {}

private:
   // virtual methods from AuthServerSession

   virtual void SetupSession() override;

   virtual void OnConnectionClosing() override;

private:
   /// session manager
   SessionManager& m_sessionManager;

   /// server model
   ServerModel m_serverModel;

   /// world model
   IModel& m_worldModel;
};
