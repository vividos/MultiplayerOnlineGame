//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Session.cpp Session base class
//

// includes
#include "stdafx.h"
#include "Session.hpp"
#include "Message.hpp"
#include "RawMessage.hpp"
#include "ByteStream.hpp"
#include "IEncryptModule.hpp"
#include <limits>
#include <cstdio>
#include <tchar.h>
#include <ulib/Exception.hpp>
#include <boost/bind.hpp>

Session::~Session()
{
   Close();
}

void Session::Start()
{
   // check if session object is created using shared_ptr; note: we can't check this in ctor
   // since when creating the object before putting it into shared_ptr, it doesn't have a
   // valid lockable weak pointer.
#ifdef _DEBUG
   // Session object or derived class object must not be created on stack!
   try
   {
      shared_from_this();
   }
   catch(std::bad_weak_ptr&)
   {
      ATLASSERT(false);
   }
#endif

   SharedMutableBuffer buffer(1024);

   m_socket.async_read_some(buffer,
      boost::bind(&Session::HandleRead, shared_from_this(),
         boost::asio::placeholders::error,
         buffer,
         boost::asio::placeholders::bytes_transferred));
}

void Session::Close()
{
   // note: don't call cancel() here, close() will cancel all waiting handlers
   boost::system::error_code ec;
   m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
   m_socket.close();
}

void Session::SendMessage(const Message& msg)
{
   // serialize message
   VectorStream s;
   s.Write16(msg.MessageId());
   s.Write16(0); // write size 0 here, since we don't know yet
   msg.Serialize(s);

   if (s.Size() - 4 > static_cast<size_t>(std::numeric_limits<unsigned short>::max()))
      throw Exception(_T("Session: size too short to send"), __FILE__, __LINE__);

   // add length
   {
      VectorStream strLength;
      strLength.Write16(static_cast<unsigned short>(s.Size() - 4));

      std::copy(strLength.Data().begin(), strLength.Data().end(), s.Data().begin()+2);
   }

   SharedConstBuffer buffer(s.Data().begin(), s.Data().end());

   // when an encryption module is set, call it
   if (m_spEncryptModule != NULL)
      m_spEncryptModule->EncryptWrite(buffer.Data().begin(), buffer.Data().end());

   // try sending buffer
   {
      boost::recursive_mutex::scoped_lock lock(m_mtxWriteQueue);

      bool bWriteInProgress = !m_deqWriteQueue.empty();
      m_deqWriteQueue.push_back(buffer);

      if (!bWriteInProgress)
      {
         /// add message to queue; will be sent on next HandleWrite call
         boost::asio::async_write(m_socket, m_deqWriteQueue.front(),
            boost::bind(&Session::HandleWrite, shared_from_this(), boost::asio::placeholders::error));
      }
   }
}

void Session::HandleRead(const boost::system::error_code& error, SharedMutableBuffer recvBuffer, size_t uiBytesTransferred)
{
   if (!error)
   {
      // must be at least 4 bytes long
      if (uiBytesTransferred < 4)
      {
         LOG_ERROR(_T("Session received too few bytes for complete message"), Log::Session);

         OnConnectionClosing();
         return; // note: with this return, the session is closed
      }

      // when an encryption module is set, call it
      if (m_spEncryptModule != NULL)
         m_spEncryptModule->DecryptRead(recvBuffer.Data().begin(), recvBuffer.Data().begin() + uiBytesTransferred);

      if (!ProcessMessageBuffer(recvBuffer.Data(), uiBytesTransferred))
         return; // close session

      // set up new read handler, using new buffer
      SharedMutableBuffer buffer(1024);

      m_socket.async_read_some(buffer,
         boost::bind(&Session::HandleRead, shared_from_this(),
            boost::asio::placeholders::error,
            buffer,
            boost::asio::placeholders::bytes_transferred));
   }
   else
   {
      if (error == boost::asio::error::eof)
      {
         // tcp/ip connection close
         LOG_INFO(_T("Session ended due to local connection close in read handler"), Log::Session);
      }
      else
      if (error == boost::asio::error::operation_aborted)
      {
         // tcp/ip connection close
         LOG_INFO(_T("Session ended due to remote connection close in read handler"), Log::Session);
      }
      else
      {
         CString cszMessage;
         cszMessage.Format(_T("Session ended due to read error: %i"), error.value());
         LOG_INFO(cszMessage, Log::Session);
      }

      OnConnectionClosing();

      // just don't register any async operation here, and the session goes out of scope
   }
}

bool Session::ProcessMessageBuffer(const std::vector<unsigned char>& vecRecvBuffer, size_t uiBytesTransferred)
{
   // try to read messages
   ConstVectorRefStream inStream(vecRecvBuffer);

   // loop to read all messages
   size_t uiOffset = 0;
   do
   {
      unsigned short usMessageId = inStream.Read16();
      unsigned short usLength = inStream.Read16();
      if (static_cast<size_t>(usLength) + 4 > uiBytesTransferred)
      {
         LOG_ERROR(_T("Session received incomplete message"), Log::Session);
         return false; // note: with this return, the session is closed
      }

      uiBytesTransferred -= static_cast<size_t>(usLength) + 4;

      // deserialize message
      RawMessage rawMessage(usMessageId, &vecRecvBuffer[uiOffset + 4], usLength);

      try
      {
         if (rawMessage.MessageId() == msgLogoutRequest)
         {
            LOG_INFO(_T("Session logging out"), Log::Session);

            OnConnectionClosing();
            return false;
         }

         bool bHandledMessage = OnReceiveMessage(rawMessage);
         if (!bHandledMessage)
         {
            CString cszText;
            cszText.Format(_T("Session received unhandled message, id=0x%04x"), rawMessage.MessageId());
            LOG_WARN(cszText, Log::Session);

            OnConnectionClosing();
            return false;
         }
      }
      catch(...)
      {
         // exception means: shut down session
         LOG_WARN(_T("Session shutting down; caught exception in message handler"), Log::Session);

         OnConnectionClosing();
         return false;
      }

      uiOffset += usLength + 4;

   } while(uiBytesTransferred > 0);

   return true;
}

void Session::HandleWrite(const boost::system::error_code& error)
{
   if (!error)
   {
      boost::recursive_mutex::scoped_lock lock(m_mtxWriteQueue);

      m_deqWriteQueue.pop_front();
      if (!m_deqWriteQueue.empty())
      {
         // as queue is still filled, send another write request
         // this ensures that packets are sent in order of SendMessage() calls
         boost::asio::async_write(m_socket, m_deqWriteQueue.front(),
            boost::bind(&Session::HandleWrite, shared_from_this(), boost::asio::placeholders::error));
      }
   }
   else
   {
      if (error == boost::asio::error::eof)
      {
         // tcp/ip connection close
         LOG_INFO(_T("Session ended due to connection close in write handler"), Log::Session);
      }
      else
      {
         CString cszMessage;
         cszMessage.Format(_T("Session ended due to write error: %hs"), error.message().c_str());
         LOG_INFO(cszMessage, Log::Session);
      }

      OnConnectionClosing();

      // just don't register any async operation here, and the session goes out of scope
   }
}

CString Session::GetSessionLogCategory(const CString& cszBaseLogCategory)
{
   CString cszCategory;
   if (Socket().is_open())
      cszCategory.Format(_T("%s.%u"), cszBaseLogCategory, Socket().local_endpoint().port());
   else
      cszCategory = cszBaseLogCategory;

   return cszCategory;
}

void Session::SetEncryptModule(std::shared_ptr<IEncryptModule> spEncryptModule)
{
   ATLASSERT(spEncryptModule != NULL);

   m_spEncryptModule = spEncryptModule;
}
