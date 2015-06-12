//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Session.hpp Session base class
//
#pragma once

// includes
#include "Network.hpp"
#include <boost/thread/recursive_mutex.hpp>
#include "SharedBuffer.hpp"
#include "ISession.hpp"
#include <deque>
#include <vector>

// forward references
class RawMessage;
class IEncryptModule;

/// connection session
class NETWORK_DECLSPEC Session:
   public ISession,
   public std::enable_shared_from_this<Session>
{
public:
   /// ctor
   Session(boost::asio::io_service& ioService)
      :m_socket(ioService)
   {
   }

   /// dtor
   ~Session();

   /// starts session
   virtual void Start();

   /// closes session
   void Close();

   /// returns session socket
   boost::asio::ip::tcp::socket& Socket()
   {
      return m_socket;
   }

   // virtual methods from ISession

   /// sends message to recipient
   virtual void SendMessage(const Message& msg) override;

protected:
   /// processes incoming message buffer
   bool ProcessMessageBuffer(const std::vector<unsigned char>& vecRecvBuffer, size_t uiBytesTransferred);

   /// called when receiving message
   virtual bool OnReceiveMessage(RawMessage& msg) = 0;

   /// called when connection is about to be closed
   virtual void OnConnectionClosing() {}

   /// returns log category for session
   CString GetSessionLogCategory(const CString& cszBaseLogCategory);

   /// sets encryption module
   void SetEncryptModule(std::shared_ptr<IEncryptModule> spEncryptModule);

private:
   /// called when a read command was completed
   void HandleRead(const boost::system::error_code& error, SharedMutableBuffer recvBuffer, size_t uiBytesTransferred);

   /// called when a write command was completed
   void HandleWrite(const boost::system::error_code& error);

private:
   /// session socket
   boost::asio::ip::tcp::socket m_socket;

   /// encryption module
   std::shared_ptr<IEncryptModule> m_spEncryptModule;

   /// mutex to protect m_deqWriteQueue access
   boost::recursive_mutex m_mtxWriteQueue;

   /// write queue
   std::deque<SharedConstBuffer> m_deqWriteQueue;
};
