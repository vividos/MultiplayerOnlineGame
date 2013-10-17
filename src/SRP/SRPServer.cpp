//
// SRP xint - SRP implementation using boost::xint
// Copyright (C) 2011-2013 Michael Fink
//
//! \file SRPServer.cpp Server class for SRP authentication
//

// includes
#include "stdafx.h"
#include "SRPServer.hpp"
#include "SRPHelper.hpp"

using namespace SRP;

const BigInteger& Server::GetB(const BigInteger& PassVerifier, const BigInteger& b)
{
   m_b = b;

   // k = SHA1(N | PAD(g))
   BigInteger k = Helper::CalcLowerK(m_gp);

   // k = 7556AA045AEF2CDD07ABAF0F665C3E818913186F
   // v = 7E273DE8696FFC4F4E337D05B4B375BEB0DDE1569E8FA00A9886D8129BADA1F1822223CA1A605B530E379BA4729FDC59F105B4787E5186F5C671085A1447B52A48CF1970B4FB6F8400BBF4CEBFBB168152E08AB5EA53D15C1AFF87B2B9DA6E04E058AD51CC72BFC9033B564E26480D78E955A5E29E7AB245DB2BE315E2099AFB
   // g = 2
   // b = E487CB59D31AC550471E81F00F6928E01DDA08E974A004F49E61F5D105284D20
   // N = EEAF0AB9ADB38DD69C33F80AFA8FC5E86072618775FF3C0B9EA2314C9C256576D674DF7496EA81D3383B4813D692C6E0E0D5D8E250B98BE48E495C1D6089DAD15DC7D7B46154D6B6CE8EF4AD69B15D4982559B297BCF1885C529F566660E57EC68EDBC3C05726CC02FD4CBF4976EAA9AFD5138FE8376435B9FC61D2FC0EB06E3

   // k*v = 39D2A44238CC9017300647C9D9171B1468AE0A4C84F6AF97B31CD3D3885C1B339CEB107E5ED1CC92A691BB0D793C0490FDC35F114DD118E61A43A11CE6CE818D33A055DC3BF968E602D2B87D729D375E246BD3EFF39FBB4C3EE0017F2F6DC4994EB607D827909A1FC34D4B0D49D0A8FF88F51452EE304F0EA1C5A18FEC326813942DBFB11B9E0B4C6D980D4CA47C50CFBC52BAD5
   // k*v % N = EB79375AF210A4866008887DC6F509260EF5B3C978827CAF5BDD25473B9CBF75AB1CDFD2C6BFCDB29DF560932FAF7E32335C5BAF113B82E9BF4799FB88D615A27DE69033F2077794CBE78425315ADCF8117726B2C11BFDB5CDC23600164BD29BE36DB46FDD40BB911CAB3254E0EF003368FC68E874BAD3F75FB1D570A840FF18

   // g^b % N = C04234AFE80C155CF2FBB1873555D1EF9A934FA578712A3D47FE3C171B7C2F66079EA008DDFBD9D9F4DDFDD2CA52E28863FB492BB7B5F5943B6F62662B264B3FB0A82535FA7EE7A7DA7F9D07C641F61FECB37020F1BBB93F93C81959D84613CA858312FF46E9196FA42DDA5168FA59167F94E2CDE621C19CE8F842BF63BB8323

   // (k*v % N) + (g^b % N) = 1,ABBB6C0ADA1CB9E353043A04FC4ADB15A989036EF0F3A6ECA3DB615E5718EEDBB2BB7FDBA4BBA78C92D35E65FA0260BA9757A4DAC8F1787DFAB6FC61B3FC60E22E8EB569EC865F3CA667212CF79CD317FE2A96D3B2D7B6F5618A4F59EE91E66668F0C76F2429D500C0D90CA649E95949E8914BB65ADC959448AA18300BFC823B
   // ((k*v % N) + (g^b % N)) % N = BD0C61512C692C0CB6D041FA01BB152D4916A1E77AF46AE105393011BAF38964DC46A0670DD125B95A981652236F99D9B681CBF87837EC996C6DA04453728610D0C6DDB58B318885D7D82C7F8DEB75CE7BD4FBAA37089E6F9C6059F388838E7A00030B331EB76840910440B1B27AAEAEEB4012B7D7665238A8E3FB004B117B58
   // ((k*v % N) + (g^b % N)) - N = BD0C61512C692C0CB6D041FA01BB152D4916A1E77AF46AE105393011BAF38964DC46A0670DD125B95A981652236F99D9B681CBF87837EC996C6DA04453728610D0C6DDB58B318885D7D82C7F8DEB75CE7BD4FBAA37089E6F9C6059F388838E7A00030B331EB76840910440B1B27AAEAEEB4012B7D7665238A8E3FB004B117B58

   // B == BD0C61512C692C0CB6D041FA01BB152D4916A1E77AF46AE105393011BAF38964DC46A0670DD125B95A981652236F99D9B681CBF87837EC996C6DA04453728610D0C6DDB58B318885D7D82C7F8DEB75CE7BD4FBAA37089E6F9C6059F388838E7A00030B331EB76840910440B1B27AAEAEEB4012B7D7665238A8E3FB004B117B58

   // formula in RFC: B = k*v + g^b % N
   // we actually want: B = ((k*v) % N) + ((g^b) % N) % N
   BigInteger temp1 = (k * PassVerifier) % m_gp.N;
   BigInteger temp2 = boost::xint::powmod(m_gp.g, b, m_gp.N);

   //m_B = (temp1 + temp2) % m_gp.N;
   // the previous operation can also be written as:
   m_B = temp1 + temp2;
   if (m_B > m_gp.N)
      m_B -= m_gp.N;

   return m_B;
}

void Server::CalcSessionKey(const BigInteger& A, const BigInteger& s, const std::string& strUsername,
                            const BigInteger& PassVerifier)
{
   m_A = A;
   m_s = s;
   m_strUsername = strUsername;

   // u = SHA1(PAD(A) | PAD(B))
   BigInteger u = Helper::CalcLowerU(A, m_B, m_gp.uiNumBits/8);

   if (u == 0)
      throw std::runtime_error("u == 0");

   // calculate session key
   // RFC says: S = (A * v^u) ^ b % N
   // but we actually want to do:
   // t1 = A * ((v^u) % N)
   // t2 = (t1 * A) % N)
   // S = (t2 ^ b) % N
   BigInteger S;
   {
      BigInteger temp1 = boost::xint::powmod(PassVerifier, u, m_gp.N);
      BigInteger temp2 = boost::xint::mulmod(A, temp1, m_gp.N);

      // reject t2 == {0, 1}
      if (temp2 <= 1)
         throw std::runtime_error("A*v^u <= 1");

      // reject t2 % N == -1
      if (temp2 % m_gp.N == -1)
         throw std::runtime_error("A*v^u % N == -1");

      S = boost::xint::powmod(temp2, m_b, m_gp.N);
   }

   // calculate K
   // K = H(S)
   BinaryData Sbin;
   Helper::ToBinaryData(S, Sbin);

   m_K = Helper::Sha1(Sbin);
}

BinaryData Server::VerifyClient(const BinaryData& Mclient)
{
   // check M1
   BinaryData Mcalculated = Helper::CalcM1(m_gp, m_strUsername, m_s, m_A, m_B, m_K);
   if (!Helper::CompareBinaryData(Mclient, Mcalculated))
      throw std::runtime_error("wrong M1");

   // calculate M2
   return Helper::CalcM2(Mclient, m_A, m_K);
}
