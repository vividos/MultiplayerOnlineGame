//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file DatabaseAuthManager.cpp Database authentication manager
//

// includes
#include "StdAfx.h"
#include "DatabaseAuthManager.hpp"
#include "DatabaseManager.hpp"
#include "AuthException.hpp"
#include "SRPServerAuthModule.hpp"
#include "StringTools.hpp"
#include "IDatabaseInterface.hpp"
#include <functional>

bool DatabaseAuthManager::IsAccountAvail(const CString& cszUsername) throw()
{
   std::shared_ptr<Database::IDatabase> spDatabase = m_databaseManager.GetDatabase();

   // select from database
   std::shared_ptr<Database::ICommand> spCommand =
      spDatabase->OpenQuery(_T("select id from account where username=?"));

   ATLASSERT(1 == spCommand->GetParamCount());

   // parameter indices are 0-based
   spCommand->SetParam(0, cszUsername);

   std::shared_ptr<Database::IResultSet> spResultSet;
   spCommand->Execute(spResultSet);

   if (spResultSet == NULL)
      return false;

   ATLASSERT(1 == spResultSet->GetColumnCount());

   int iId = 0;
   spResultSet->GetValue(0, iId);

   return iId != 0;
}

Account DatabaseAuthManager::GetAccount(const CString& cszUsername)
{
   std::shared_ptr<Database::IDatabase> spDatabase = m_databaseManager.GetDatabase();

   // insert into database
   std::shared_ptr<Database::ICommand> spCommand =
      spDatabase->OpenQuery(_T("select active, password, salt, id from account where username=?"));

   ATLASSERT(1 == spCommand->GetParamCount());
   spCommand->SetParam(0, cszUsername);

   std::shared_ptr<Database::IResultSet> spResultSet;
   spCommand->Execute(spResultSet);

   if (spResultSet == NULL)
      throw AuthException(AuthException::authUnknownUser, _T("account not found in db"), __FILE__, __LINE__);

   ATLASSERT(4 == spResultSet->GetColumnCount());

   // get values
   int iActive = 0;
   spResultSet->GetValue(0, iActive);

   CString cszPassVerifier;
   spResultSet->GetValue(1, cszPassVerifier);

   CString cszSalt;
   spResultSet->GetValue(2, cszSalt);

   if (iActive == 0)
      throw AuthException(AuthException::authInactiveUser, _T("account not active"), __FILE__, __LINE__);

   int iAccountId = 0;
   spResultSet->GetValue(3, iAccountId);

   // create account object
   Account a;
   a.Username(cszUsername);
   a.Password(cszPassVerifier);
   a.Salt(cszSalt);
   a.AccountId(iAccountId);

   return a;
}

std::shared_ptr<IServerAuthModule> DatabaseAuthManager::GetAuthenticationModule()
{
   SRP::ServerAuthModule::T_fnGetServerAuthInfo fnGetServerAuthInfo =
      std::bind(&DatabaseAuthManager::GetServerAuthInfo,
      this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

   return std::shared_ptr<IServerAuthModule>(
      new SRP::ServerAuthModule(fnGetServerAuthInfo));
}

void DatabaseAuthManager::GetServerAuthInfo(const CString& cszUsername,
   std::vector<unsigned char>& vecPasswordKey, std::vector<unsigned char>& vecSalt, int& iAccountId)
{
   Account a = GetAccount(cszUsername);

   StringTools::HexStringToBytes(a.Password(), vecPasswordKey);
   StringTools::HexStringToBytes(a.Salt(), vecSalt);
   iAccountId = a.AccountId();
}
