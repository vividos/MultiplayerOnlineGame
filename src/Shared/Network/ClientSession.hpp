//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ClientSession.hpp Client session
//
#pragma once

// includes
#include "Network.hpp"
#include "Session.hpp"
#include <ulib/thread/Event.hpp>
#include <functional>
#include "LogoutMessage.hpp"

/// \brief client session
/// \details a network session on the client side
class NETWORK_DECLSPEC ClientSession: public Session
{
public:
   DEFINE_INSTANCE(ClientSession)

   /// \brief connection states
   /// \details the following transitions occur:
   /// \dot
   /// digraph localmodel {
   ///    node [shape=box]
   ///    connectStateNotConnected -> connectStateLookupHostname
   ///    connectStateLookupHostname -> connectStateLookupHostnameError
   ///    connectStateLookupHostname -> connectStateConnecting
   ///    connectStateConnecting -> connectStateConnectError
   ///    connectStateConnecting -> connectStateConnectSuccess
   ///    connectStateConnectSuccess -> connectStateAuthenticate
   ///    connectStateAuthenticate -> connectStateAuthFailed
   ///    connectStateAuthenticate -> connectStateLoggedIn
   ///    connectStateLoggedIn -> connectStateInitialized
   ///    connectStateLoggedIn -> connectStateLoggedOut
   /// }
   /// \enddot
   enum T_enConnectState
   {
      connectStateNotConnected = 0,    ///< not connected with server or canceled by user
      connectStateLookupHostname = 1,  ///< looking up hostname
      connectStateLookupHostnameError = 2,   ///< error while looking up hostname
      connectStateConnecting = 3,      ///< connecting
      connectStateConnectError = 4,    ///< connect error
      connectStateConnectSuccess = 5,  ///< connect success
      connectStateAuthenticate = 6,    ///< authenticate in progress (when auth module is configured)
      connectStateAuthFailed = 7,      ///< authenticate failed (when auth module is configured)
      connectStateLoggedIn = 8,        ///< login succeeded
      connectStateLoggedOut = 9,       ///< logged out
      connectStateInitialized = 10,    ///< model has been initialized
   };

   /// callback function to update connect state
   typedef std::function<void(T_enConnectState)> T_fnOnConnectStateChanged;

   /// callback function called on client logout
   typedef std::function<void(LogoutMessage::T_enLogoutReason)> T_fnOnLogout;

   /// ctor
   ClientSession(boost::asio::io_service& ioService);

   /// dtor
   virtual ~ClientSession();

   /// sets handler for connect state changes
   void SetHandlerOnConnectStateChanged(T_fnOnConnectStateChanged fnOnConnectStateChanged = T_fnOnConnectStateChanged());

   /// sets handler for logout
   void SetHandlerOnLogout(T_fnOnLogout fnOnLogout = T_fnOnLogout());

   /// starts connecting to server
   void Connect(const CString& cszServer, unsigned short usPort);

   /// cancel connecting
   void StopConnect();

   /// disconnects from server
   void Disconnect();

   /// returns current connect state
   T_enConnectState ConnectState() const { return m_enConnectState; }

protected:
   /// message handler
   virtual bool OnReceiveMessage(RawMessage& rawMessage) override;

   /// called when connection is about to be closed
   virtual void OnConnectionClosing() override;

   /// called on successful connection
   virtual void OnConnectSuccess() {}

   /// updates current connect state
   void UpdateConnectState(T_enConnectState enConnectState);

private:
   /// handler called when resolver found an endpoint
   void OnAsyncResolve(const boost::system::error_code& error,
      boost::asio::ip::tcp::resolver::iterator endpointIter);

   /// handler called when a connect operation was successful or returned an error
   void OnConnect(const boost::system::error_code& error,
      boost::asio::ip::tcp::resolver::iterator endpointIter);

private:
   /// hostname resolver
   boost::asio::ip::tcp::resolver m_resolver;

   /// connect state
   T_enConnectState m_enConnectState;

   /// callback function for state changes
   T_fnOnConnectStateChanged m_fnOnConnectStateChanged;

   /// callback function for logout
   T_fnOnLogout m_fnOnLogout;

   /// event that signals if a socket is connected
   ManualResetEvent m_evtSocketConnected;
};
