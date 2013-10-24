//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file StaticAccountAuthManager.hpp Authentication manager with static accounts (for testing)
//
#pragma once

// includes
#include "Base.hpp"
#include "IAuthManager.hpp"
#include "Account.hpp"
#include "SRPServerAuthModule.hpp"
#include "StringTools.hpp"
#include <map>

/// auth manager that has a fixed set of accounts
class StaticAccountAuthManager: public IAuthManager
{
public:
   /// ctor
   StaticAccountAuthManager() throw() {}
   /// dtor
   virtual ~StaticAccountAuthManager() throw() {}

   /// adds new account
   void AddAccount(const Account& account)
   {
      m_mapAllAccounts.insert(std::make_pair(account.Username(), account));
   }

private:
   // virtual methods from IAuthManager

   virtual bool IsAccountAvail(const CString& cszUsername) throw() override
   {
      return m_mapAllAccounts.find(cszUsername) != m_mapAllAccounts.end();
   }

   virtual Account GetAccount(const CString& cszUsername) override
   {
      if (!IsAccountAvail(cszUsername))
         throw Exception(_T("unknown user"), __FILE__, __LINE__);

      return m_mapAllAccounts[cszUsername];
   }

   virtual std::shared_ptr<IServerAuthModule> GetAuthenticationModule() override
   {
      SRP::ServerAuthModule::T_fnGetServerAuthInfo fnGetServerAuthInfo =
         std::bind(&StaticAccountAuthManager::GetServerAuthInfo,
         this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

      return std::shared_ptr<IServerAuthModule>(
         new SRP::ServerAuthModule(fnGetServerAuthInfo));
   }

   void GetServerAuthInfo(const CString& cszUsername, std::vector<unsigned char>& vecPasswordKey,
      std::vector<unsigned char>& vecSalt, int& iAccountId)
   {
      Account a = GetAccount(cszUsername);

      StringTools::HexStringToBytes(a.Password(), vecPasswordKey);
      StringTools::HexStringToBytes(a.Salt(), vecSalt);
      iAccountId = a.AccountId();
   }

private:
   /// map with all accounts
   std::map<CString, Account> m_mapAllAccounts;
};
