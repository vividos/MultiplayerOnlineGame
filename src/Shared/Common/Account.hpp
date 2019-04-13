//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Account.hpp User account data
//
#pragma once

/// account data
class Account
{
public:
   /// ctor
   Account()
      :m_bLogonAllowed(false),
       m_iAccountId(-1)
   {
   }

   // get methods

   /// returns username
   const CString& Username() const { return m_cszUsername; }

   /// returns password or encrypted password key
   const CString& Password() const { return m_cszPassword; }

   /// returns salt of password key
   const CString& Salt() const { return m_cszSalt; }

   /// returns if login is allowed
   bool LogonAllowed() const { return m_bLogonAllowed; }

   /// returns account id
   int AccountId() const { return m_iAccountId; }

   // set methods

   /// sets username
   void Username(const CString& cszUsername) { m_cszUsername = cszUsername; }

   /// sets password or encrypted password key
   void Password(const CString& cszPassword) { m_cszPassword = cszPassword; }

   /// sets salt of password key
   void Salt(const CString& cszSalt) { m_cszSalt = cszSalt; }

   /// sets if login is allowed
   void LogonAllowed(bool bLogonAllowed) { m_bLogonAllowed = bLogonAllowed; }

   /// sets account id
   void AccountId(int iAccountId) { m_iAccountId = iAccountId; }

private:
   CString m_cszUsername;  ///< username
   CString m_cszPassword;  ///< password or encrypted password key
   CString m_cszSalt;      ///< salt of password key
   bool m_bLogonAllowed;   ///< indicates if login is allowed
   int m_iAccountId;       ///< account id
};
