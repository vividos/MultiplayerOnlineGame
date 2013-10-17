//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file TestSRPxint.cpp Unit tests for SRP classes
//

// includes
#include "stdafx.h"
#include "SRPServer.hpp"
#include "SRPClient.hpp"
#include "SRPHelper.hpp"
//#include "HighResolutionTimer.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SRP;

namespace UnitTest
{

// test vectors from RFC 5054, Appendix B

// preset index; 1024-bit prime N
const unsigned int c_uiPresetIndex = 0;

/// username I
LPCSTR c_pszaUsername = "alice";

/// password p
LPCSTR c_pszaPassword = "password123";

/// Salt, s = BEB25379 D1A8581E B5A72767 3A2441EE
const char* c_pszaSalt = "BEB25379D1A8581EB5A727673A2441EE";

/// server secret, b = E487CB59 D31AC550 471E81F0 0F6928E0 1DDA08E9 74A004F4 9E61F5D1 05284D20
const char* c_pszaSmallB = "E487CB59D31AC550471E81F00F6928E01DDA08E974A004F49E61F5D105284D20";

/// client secret, a = 60975527 035CF2AD 1989806F 0407210B C81EDC04 E2762A56 AFD529DD DA2D4393
const char* c_pszaSmallA = "60975527035CF2AD1989806F0407210BC81EDC04E2762A56AFD529DDDA2D4393";

/// server public key, B
const char* c_pszaB = "BD0C61512C692C0CB6D041FA01BB152D4916A1E77AF46AE105393011BAF38964DC46A0670DD125B95A981652236F99D9B681CBF87837EC996C6DA04453728610D0C6DDB58B318885D7D82C7F8DEB75CE7BD4FBAA37089E6F9C6059F388838E7A00030B331EB76840910440B1B27AAEAEEB4012B7D7665238A8E3FB004B117B58";


class TestServer;

/// test client class
class TestClient
{
public:
   TestClient(Client& client)
      :m_client(client),
       m_bRandom(false)
   {
   }

   void EnableRandom(bool bEnable){ m_bRandom = bEnable; }

   void SendMessage1(TestServer& server, const std::string& strUsername, const std::string& strPassword);
   void ReceiveMessage2(TestServer& server, BigInteger s, BigInteger B);
   void ReceiveMessage4(const BinaryData& Ms);

private:
   Client& m_client;
   bool m_bRandom;
   std::string m_strUsername;
   std::string m_strPassword;
   BigInteger m_A;
};

/// test server
class TestServer
{
public:
   TestServer(Server& server, const std::string& strUsername, const std::string& strPassword)
      :m_server(server),
       m_bRandom(false),
       m_bCacheClientKey(false),
       m_strUsername(strUsername),
       m_strPassword(strPassword)
   {
   }

   void EnableRandom(bool bEnable){ m_bRandom = bEnable; }
   void EnablCacheClientKey(bool bEnable){ m_bCacheClientKey = bEnable; }

   void ReceiveMessage1(TestClient& client, const std::string& strUsername, BigInteger A);
   void ReceiveMessage3(TestClient& client, BigInteger A, const BinaryData& Mc);

private:
   Server& m_server;
   bool m_bRandom;
   bool m_bCacheClientKey;
   BigInteger m_CachedPassVerifier;
   std::string m_strUsername;
   std::string m_strPassword;
};


void TestClient::SendMessage1(TestServer& server, const std::string& strUsername, const std::string& strPassword)
{
   m_strUsername = strUsername;
   m_strPassword = strPassword;

   BigInteger a;
   if (m_bRandom)
   {
      boost::xint::strong_random_generator srg;

      a = boost::xint::integer::random_by_size(srg,
         256,
         false, // high_bit_on
         false, // low_bit_on
         false);// can_be_negative
   }
   else
      a = BigInteger(c_pszaSmallA, 16);

   m_A = m_client.GetA(a);

   // Client -> Server: send I (Username), A
   server.ReceiveMessage1(*this, strUsername, m_A);
}

void TestServer::ReceiveMessage1(TestClient& client, const std::string& strUsername, BigInteger A)
{
   BigInteger s(c_pszaSalt, 16);

   if (m_strUsername != strUsername)
      throw std::runtime_error("invalid user");

   BigInteger PassVerifier;
   if (!m_bCacheClientKey || (m_CachedPassVerifier == BigInteger()))
   {
      GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);
      PassVerifier = Helper::GeneratePassword(strUsername, m_strPassword, gp, s);

      if (m_bCacheClientKey)
         m_CachedPassVerifier = PassVerifier;
   }
   else
      PassVerifier = m_CachedPassVerifier;

   BigInteger b;
   if (m_bRandom)
   {
      boost::xint::strong_random_generator srg;

      b = boost::xint::integer::random_by_size(srg,
         256,
         false, // high_bit_on
         false, // low_bit_on
         false);// can_be_negative
   }
   else
      b = BigInteger(c_pszaSmallB, 16);

   BigInteger B = m_server.GetB(PassVerifier, b);

   m_server.CalcSessionKey(A, s, strUsername, PassVerifier);

   // Server -> Client: send s, B
   client.ReceiveMessage2(*this, s, B);
}

void TestClient::ReceiveMessage2(TestServer& server, BigInteger s, BigInteger B)
{
   BinaryData Mclient = m_client.CalcM(m_strUsername, m_strPassword, s, B);

   // Client -> Server: send A, Mc
   server.ReceiveMessage3(*this, m_A, Mclient);
}

void TestServer::ReceiveMessage3(TestClient& client, BigInteger A, const BinaryData& Mc)
{
   BinaryData Ms = m_server.VerifyClient(Mc);

   // Server -> Client: send Ms
   client.ReceiveMessage4(Ms);
}

void TestClient::ReceiveMessage4(const BinaryData& Ms)
{
   if (!m_client.VerifyServer(Ms))
      throw std::runtime_error("verify server failed");
}


/// tests SRP classes
TEST_CLASS(TestSRPxint)
{
public:
   static bool QuickCheckNumber(SRP::BigInteger n, unsigned int uiLenInBits,
      BYTE bFirst, BYTE bSecond, BYTE bBeforeLast, BYTE bLast)
   {
      BinaryData data;
      Helper::ToBinaryData(n, data);
      return QuickCheckNumber(data, uiLenInBits,
         bFirst, bSecond, bBeforeLast, bLast);
   }

   static bool QuickCheckNumber(const SRP::BinaryData& data, unsigned int uiLenInBits,
      BYTE bFirst, BYTE bSecond, BYTE bBeforeLast, BYTE bLast)
   {
      size_t uiLen = data.size();

      return (uiLen == uiLenInBits/8) &&
         data[0] == bFirst &&
         data[1] == bSecond &&
         data[uiLen-2] == bBeforeLast &&
         data[uiLen-1] == bLast;
   }

   /// tests group parameter
   TEST_METHOD(TestGroupParameter)
   {
      GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);

      // check if it's the right prime and generator
      Assert::IsTrue(gp.g == 2);

      Assert::IsTrue(QuickCheckNumber(gp.N, 1024, 0xee, 0xaf, 0x06, 0xe3));
   }

   /// tests converting salt string to binary data
   TEST_METHOD(TestSaltToBinary)
   {
      BigInteger s(c_pszaSalt, 16);

      Assert::IsTrue(QuickCheckNumber(s, 128, 0xbe, 0xb2, 0x41, 0xee));
   }

   /// tests password verifier v
   TEST_METHOD(TestPasswordVerifier)
   {
      std::string strUsername(c_pszaUsername);
      std::string strPassword(c_pszaPassword);

      BigInteger s(c_pszaSalt, 16);

      GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);

      // generate password verifier
      BigInteger PassVerifier = Helper::GeneratePassword(strUsername, strPassword, gp, s);

      // check password verifier v = 7E273DE8 ... E2099AFB
      Assert::IsTrue(QuickCheckNumber(PassVerifier, 1024, 0x7E, 0x27, 0x9A, 0xFB));
   }

   /// tests calculating B value on server side 
   TEST_METHOD(TestCalcB)
   {
      std::string strUsername(c_pszaUsername);
      std::string strPassword(c_pszaPassword);

      GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);

      Server server(gp);

      BigInteger s(c_pszaSalt, 16);
      BigInteger PassVerifier = Helper::GeneratePassword(strUsername, strPassword, gp, s);

      BigInteger b(c_pszaSmallB, 16);
      BigInteger B = server.GetB(PassVerifier, b);

      // check B = BD0C6151 ... 4B117B58
      Assert::IsTrue(QuickCheckNumber(B, 1024, 0xbd, 0x0c, 0x7b, 0x58));
   }

   /// tests calculating A value for client
   TEST_METHOD(TestCalcA)
   {
      GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);

      Client c(gp);

      BigInteger a(c_pszaSmallA, 16);
      BigInteger A = c.GetA(a);

      // check A = 61D5E490 ... 0769447B
      Assert::IsTrue(QuickCheckNumber(A, 1024, 0x61, 0xd5, 0x44, 0x7b));
   }

   /// tests complete key exchange
   TEST_METHOD(TestKeyExchange1)
   {
      std::string strUsername(c_pszaUsername);
      std::string strPassword(c_pszaPassword);

      GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);

      Client client(gp);
      // Client -> Server: I (Username), A
      BigInteger a(c_pszaSmallA, 16);
      BigInteger A = client.GetA(a);

      // server will abort if it detects that A % N == 0
      Assert::IsTrue((A % gp.N) != 0);



      // Server -> Client: send N, g, s, B
      // for convenience, the protocol assumes N, g and only sends s, B
      Server server(gp);

      BigInteger s(c_pszaSalt, 16);
      BigInteger PassVerifier = Helper::GeneratePassword(strUsername, strPassword, gp, s);

      BigInteger b(c_pszaSmallB, 16);
      BigInteger B = server.GetB(PassVerifier, b);



      // Client -> Server: send A, Mc (client must show his M first)
      BinaryData Mclient = client.CalcM(strUsername, strPassword, s, B);

      // Server: check client's Mc
      server.CalcSessionKey(A, s, strUsername, PassVerifier);

      BinaryData Mserver;
      try
      {
         Mserver = server.VerifyClient(Mclient);
      }
      catch(std::exception&)
      {
         Assert::Fail();
      }


      // Server -> Client: send Ms
      Assert::IsTrue(true == client.VerifyServer(Mserver));



      // check Kc and Ks
      BinaryData Kclient = client.GetK();
      BinaryData Kserver = server.GetK();

      Assert::IsTrue(true == Helper::CompareBinaryData(Kclient, Kserver));

      // check K == 017eefa1 ... 0f1b11bb
      Assert::IsTrue(QuickCheckNumber(Kclient, 160, 0x01, 0x7e, 0x11, 0xbb));
   }

   /// tests calculating k from group parameters
   TEST_METHOD(TestCalcLowerK)
   {
      GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);

      // k = SHA1(N | PAD(g))
      BigInteger k = Helper::CalcLowerK(gp);

      Assert::IsTrue(k == BigInteger("7556AA045AEF2CDD07ABAF0F665C3E818913186F", 16));
   }

   /// tests calculating K on client side
   TEST_METHOD(TestCalcK)
   {
      std::string strUsername(c_pszaUsername);
      std::string strPassword(c_pszaPassword);

      GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);

      Client client(gp);

      BigInteger s(c_pszaSalt, 16);

      // set a
      BigInteger a(c_pszaSmallA, 16);
      client.GetA(a);

      // set B
      BigInteger B(c_pszaB, 16);

      // calc M, K
      BinaryData Mclient = client.CalcM(strUsername, strPassword, s, B);
      BinaryData Kclient = client.GetK();

      // check K == 017eefa1 ... 0f1b11bb
      Assert::IsTrue(QuickCheckNumber(Kclient, 160, 0x01, 0x7e, 0x11, 0xbb));
   }

   /// tests client and server exchanging K
   TEST_METHOD(TestKeyExchange2)
   {
      GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);

      Client client(gp);
      Server server(gp);

      std::string strUsername(c_pszaUsername);
      std::string strPassword(c_pszaPassword);

      TestClient tc(client);
      TestServer ts(server, strUsername, strPassword);

      tc.SendMessage1(ts, strUsername, strPassword);

      // check Kc and Ks
      BinaryData Kclient = client.GetK();
      BinaryData Kserver = server.GetK();

      Assert::IsTrue(true == Helper::CompareBinaryData(Kclient, Kserver));
   }

   /// tests client and server who uses different group parameters
   TEST_METHOD(TestFailDifferentGroupParams)
   {
      try
      {
         GroupParameter gp1 = Helper::GetPresetGroupParameter(c_uiPresetIndex);
         GroupParameter gp2 = Helper::GetPresetGroupParameter(c_uiPresetIndex+1);

         Client client(gp1);
         Server server(gp2);

         std::string strUsername(c_pszaUsername);
         std::string strPassword(c_pszaPassword);

         TestClient tc(client);
         TestServer ts(server, strUsername, strPassword);

         tc.SendMessage1(ts, strUsername, strPassword);

         Assert::Fail();
      }
      catch(...)
      {
      }
   }

   /// tests client and server, and client uses wrong username
   TEST_METHOD(TestFailClientWrongUsername)
   {
      try
      {
         GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);

         Client client(gp);
         Server server(gp);

         std::string strUsername(c_pszaUsername);
         std::string strPassword(c_pszaPassword);

         TestClient tc(client);
         TestServer ts(server, strUsername, strPassword);

         tc.SendMessage1(ts, "otherusername", strPassword);

         Assert::Fail();
      }
      catch(...)
      {
      }
   }

   /// tests client and server, and client uses wrong password
   TEST_METHOD(TestFailClientWrongPassword)
   {
      try
      {
         GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);

         Client client(gp);
         Server server(gp);

         std::string strUsername(c_pszaUsername);
         std::string strPassword(c_pszaPassword);

         TestClient tc(client);
         TestServer ts(server, strUsername, strPassword);

         tc.SendMessage1(ts, strUsername, "wrongpassword");

         Assert::Fail();
      }
      catch(...)
      {
      }
   }

   /// tests client and server, and server has empty password
   TEST_METHOD(TestFailServerEmptyPassword)
   {
      try
      {
         GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);

         Client client(gp);
         Server server(gp);

         std::string strUsername(c_pszaUsername);
         std::string strPassword(c_pszaPassword);

         TestClient tc(client);
         TestServer ts(server, strUsername, "");

         tc.SendMessage1(ts, strUsername, strPassword);

         Assert::Fail();
      }
      catch(...)
      {
      }
   }

   /// tests client and server, and server has different user
   TEST_METHOD(TestFailServerDifferentUser)
   {
      try
      {
         GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);

         Client client(gp);
         Server server(gp);

         std::string strUsername(c_pszaUsername);
         std::string strPassword(c_pszaPassword);

         TestClient tc(client);
         TestServer ts(server, "otherusername", strPassword);

         tc.SendMessage1(ts, strUsername, strPassword);

         Assert::Fail();
      }
      catch(...)
      {
      }
   }

   /// tests client and server using random number generator (not fixed test vectors)
   TEST_METHOD(TestKeyExchangeTrueRandom)
   {
      try
      {
         GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);

         Client client(gp);
         Server server(gp);

         std::string strUsername(c_pszaUsername);
         std::string strPassword(c_pszaPassword);

         TestClient tc(client);
         tc.EnableRandom(true);

         TestServer ts(server, strUsername, strPassword);
         ts.EnableRandom(true);

         tc.SendMessage1(ts, strUsername, strPassword);
      }
      catch(...)
      {
         Assert::Fail();
      }
   }

   /// test function to generate password verifier; doesn't contain test
#if 0
   // note that this test would need HighResolutionTimer and so would link to Base.dll, but
   // the C++ Unit Test framework doesn't allow that external dlls are loaded during running
   // tests.
   TEST_METHOD(TestThroughput)
   {
      HighResolutionTimer timer;

      timer.Start();

      GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);

      Client client(gp);
      Server server(gp);

      std::string strUsername(c_pszaUsername);
      std::string strPassword(c_pszaPassword);

      TestClient tc(client);
      tc.EnableRandom(true);

      TestServer ts(server, strUsername, strPassword);
      ts.EnableRandom(true);
      ts.EnablCacheClientKey(true);

      for (unsigned int i=0;;i++)
      {
         tc.SendMessage1(ts, strUsername, strPassword);

         if (timer.Elapsed() >= 1.0)
         {
            // print out statistics
            double dElapsed = timer.Elapsed();

            ATLTRACE(_T("made %u key exchange checks in %u.%02u seconds"),
               i+1, unsigned(dElapsed), unsigned((dElapsed-unsigned(dElapsed))*100));

            timer.Restart();
            i=0;
         }
      }
   }
#endif

   /// test function to generate password verifier; doesn't contain test
   TEST_METHOD(TestGeneratePasswordVerifier)
   {
      // test to generate users
      std::string strUsername("michi");
      std::string strPassword("pass1");

      // generate salt
      boost::xint::strong_random_generator srg;

      BigInteger s = boost::xint::integer::random_by_size(srg,
         128,
         false, // high_bit_on
         false, // low_bit_on
         false);// can_be_negative

      std::string strSalt = SRP::Helper::ToString(s);

      GroupParameter gp = Helper::GetPresetGroupParameter(c_uiPresetIndex);

      // generate password verifier
      BigInteger PassVerifier = Helper::GeneratePassword(strUsername, strPassword, gp, s);

      std::string strPassVerifier = SRP::Helper::ToString(PassVerifier);

      CString cszText;
      cszText.Format(_T("   values (1, '%hs', 1, '%hs', '%hs')"),
         strUsername.c_str(), strPassVerifier.c_str(), strSalt.c_str());

      Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(cszText);
   }
};

} // namespace UnitTest
