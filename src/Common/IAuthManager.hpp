//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file IAuthManager.hpp Authentication manager interface
//
#pragma once

// includes
#include "Account.hpp"

// forward references
class IServerAuthModule;

/// interface for access to authentication manager
class IAuthManager: public boost::noncopyable
{
public:
   DEFINE_INSTANCE(IAuthManager)

   /// dtor
   virtual ~IAuthManager() throw() {}

   /// checks if account is available
   virtual bool IsAccountAvail(const CString& cszUsername) throw() = 0;

   /// returns account info
   virtual Account GetAccount(const CString& cszUsername) = 0;

   /// returns authentication module for authentication manager
   virtual boost::shared_ptr<IServerAuthModule> GetAuthenticationModule() = 0;
};
