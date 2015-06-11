//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestSessionManager.hpp Session manager for unit tests
//
#pragma once

// includes
#include "ISessionManager.hpp"
#include "Session.hpp"
#include "RawMessage.hpp"
#include <vector>

class TestSessionManager: public ISessionManager
{
public:
   TestSessionManager(boost::asio::io_service& ioService)
      :m_ioService(ioService)
   {
   }

   class TestSession: public Session
   {
   public:
      TestSession(boost::asio::io_service& ioService)
         :Session(ioService)
      {
      }

      virtual ~TestSession()
      {
      }

      virtual bool OnReceiveMessage(RawMessage& msg) override
      {
         m_vecReceivedMessages.push_back(msg);
         return true;
      }

      std::vector<RawMessage> m_vecReceivedMessages;
   };

   virtual std::shared_ptr<Session> CreateNewSession() override
   {
      return std::shared_ptr<Session>(new TestSession(m_ioService));
   }

   virtual void ConnectSession(const ObjectId& /*objId*/, std::shared_ptr<Session> /*spSession*/)
   {
      throw -1;
   }

   virtual std::weak_ptr<Session> FindSession(const ObjectId& /*objId*/) override
   {
      return std::weak_ptr<Session>();
   }

private:
   boost::asio::io_service& m_ioService;
};
