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
   Account() throw()
      :m_bLogonAllowed(false),
       m_iAccountId(-1)
   {
   }

   // get methods

   /// returns username
   const CString& Username() const throw() { return m_cszUsername; }

   /// returns password or encrypted password key
   const CString& Password() const throw() { return m_cszPassword; }

   /// returns salt of password key
   const CString& Salt() const throw() { return m_cszSalt; }

   /// returns if login is allowed
   bool LogonAllowed() const throw() { return m_bLogonAllowed; }

   /// returns account id
   int AccountId() const throw() { return m_iAccountId; }

   // set methods

   /// sets username
   void Username(const CString& cszUsername) throw() { m_cszUsername = cszUsername; }

   /// sets password or encrypted password key
   void Password(const CString& cszPassword) throw() { m_cszPassword = cszPassword; }

   /// sets salt of password key
   void Salt(const CString& cszSalt) throw() { m_cszSalt = cszSalt; }

   /// sets if login is allowed
   void LogonAllowed(bool bLogonAllowed) throw() { m_bLogonAllowed = bLogonAllowed; }

   /// sets account id
   void AccountId(int iAccountId) throw() { m_iAccountId = iAccountId; }

private:
   CString m_cszUsername;  ///< username
   CString m_cszPassword;  ///< password or encrypted password key
   CString m_cszSalt;      ///< salt of password key
   bool m_bLogonAllowed;   ///< indicates if login is allowed
   int m_iAccountId;       ///< account id
};
