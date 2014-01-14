//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AuthInfo.hpp Authentication info
//
#pragma once

/// authentication info
class AuthInfo
{
public:
   /// default ctor
   AuthInfo() throw()
   {
   }

   /// ctor that takes username and password
   AuthInfo(const CString& cszUsername, const CString& cszPassword) throw()
      :m_cszUsername(cszUsername),
       m_cszPassword(cszPassword)
   {
   }

   /// returns username
   CString Username() const throw() { return m_cszUsername; }

   /// returns password
   CString Password() const throw() { return m_cszPassword; }

private:
   CString m_cszUsername;  ///< username
   CString m_cszPassword;  ///< password
};
