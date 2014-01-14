//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AuthException.hpp Authentication error exception
//
#pragma once

// includes
#include <ulib\Exception.hpp>

/// exception for authentication failures
class AuthException: public Exception
{
public:
   /// authentication error
   enum T_enAuthError
   {
      authInternalError = 0,  ///< internal error
      authFailed = 1,         ///< authentication failed
      authUnknownUser = 2,    ///< unknown user; not in database
      authInactiveUser = 3,   ///< user is marked as inactive
   };

   /// ctor
   AuthException(T_enAuthError enAuthError, const CString& cszMessage, LPCSTR pszSourceFile, UINT uiSourceLine) throw()
      :Exception(CString(_T("Authentication error: ")) + AuthErrorText(enAuthError) + _T(": ") + cszMessage,
         pszSourceFile, uiSourceLine),
       m_enAuthError(authInternalError)
   {
   }

   /// returns authentication error
   T_enAuthError AuthError() const throw() { return m_enAuthError; }

   /// converts auth error to error text
   static LPCTSTR AuthErrorText(T_enAuthError enAuthError) throw()
   {
      switch(enAuthError)
      {
      case authInternalError: return _T("Internal error");
      default: return _T("Unknown error");
      }
   }

private:
   /// authentication error
   T_enAuthError m_enAuthError;
};
