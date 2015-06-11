#include "stdafx.h"
#include "ServerSessionImpl.hpp"
#include "SessionManager.hpp"

/// ctor
ServerSessionImpl::ServerSessionImpl(boost::asio::io_service& ioService,
   IModel& worldModel, SessionManager& sessionManager)
:AuthServerSession(ioService),
m_worldModel(worldModel),
m_serverModel(worldModel),
m_sessionManager(sessionManager)
{
}

void ServerSessionImpl::SetupSession()
{
   //m_serverModel.SetSession(shared_from_this());

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

void ServerSessionImpl::OnConnectionClosing()
{
   // unregister in session manager
   std::weak_ptr<ServerSession> wpSession =
      std::dynamic_pointer_cast<ServerSession>(shared_from_this());

   m_sessionManager.Remove(wpSession);
}
