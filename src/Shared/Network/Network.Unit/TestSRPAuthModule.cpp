//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestSRPAuthModule.cpp Unit tests for SRP authentication module
//

// includes
#include "stdafx.h"
#include "SRPServer.hpp"
#include "SRPHelper.hpp"
#include "SRPClientAuthModule.hpp"
#include "SRPServerAuthModule.hpp"
#include "AuthModuleTester.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
   LPCTSTR c_pszUsername = _T("user1");
   LPCTSTR c_pszPassword = _T("pass1");
   const unsigned char c_salt[] = { 0x12, 0x34, 0x56, 0x78 };

   const unsigned int c_uiPresetIndexGroupParameter = 0;

   void GetTestServerAuthInfo(const CString& cszUsername, std::vector<unsigned char>& vecPasswordKey,
      std::vector<unsigned char>& vecSalt, int& iAccountId)
   {
      if (cszUsername != c_pszUsername)
         throw - 1; // not our username

      vecSalt = SRP::BinaryData(c_salt, c_salt + sizeof(c_salt));

      // generate password verifier (server side)
      USES_CONVERSION;
      std::string strUsername(T2CA(c_pszUsername));
      std::string strPassword(T2CA(c_pszPassword));

      boost::xint::integer s = SRP::Helper::ToInteger<boost::xint::integer>(vecSalt);

      SRP::GroupParameter gp = SRP::Helper::GetPresetGroupParameter(c_uiPresetIndexGroupParameter);

      boost::xint::integer v = SRP::Helper::GeneratePassword(strUsername, strPassword, gp, s);

      SRP::Helper::ToBinaryData(v, vecPasswordKey);

      iAccountId = 42;
   }

   /// tests SRP::ServerAuthModule and SRP::ClientAuthModule classes
   TEST_CLASS(TestSRPAuthenticationModule)
   {
      /// tests SRP authentication using Server and Client class
      TEST_METHOD(TestAuthentication)
      {
         AuthInfo authInfo(c_pszUsername, c_pszPassword);

         SRP::ServerAuthModule serverAuthModule(&GetTestServerAuthInfo);
         SRP::ClientAuthModule clientAuthModule(authInfo);

         AuthModuleTester tester(serverAuthModule, clientAuthModule);
         Assert::IsTrue(tester.Test(), _T("Auth module tester must test the module successfully"));
      }
   };

} // namespace UnitTest
