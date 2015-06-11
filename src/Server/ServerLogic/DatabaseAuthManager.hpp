//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file DatabaseAuthManager.hpp Database authentication manager
//
#pragma once

// includes
#include "IAuthManager.hpp"

// forward references
namespace Database
{
class Manager;
}

/// authentication manager that uses database
class DatabaseAuthManager: public IAuthManager
{
public:
   /// ctor
   DatabaseAuthManager(Database::Manager& databaseManager)
      :m_databaseManager(databaseManager)
   {
   }
   /// dtor
   virtual ~DatabaseAuthManager() throw() {}

private:
   // virtual methods from IAuthManager

   /// checks if account is available
   virtual bool IsAccountAvail(const CString& cszUsername) throw() override;

   /// returns account info
   virtual Account GetAccount(const CString& cszUsername) override;

   /// returns authentication module for authentication manager
   virtual std::shared_ptr<IServerAuthModule> GetAuthenticationModule() override;

private:
   /// returns server authentication info
   void GetServerAuthInfo(const CString& cszUsername,
      std::vector<unsigned char>& vecPasswordKey, std::vector<unsigned char>& vecSalt,
      int& iAccountId);

private:
   /// database manager
   Database::Manager& m_databaseManager;
};
