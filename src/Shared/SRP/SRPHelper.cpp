//
// SRP xint - SRP implementation using boost::xint
// Copyright (C) 2011-2014 Michael Fink
//
/// \file SRPHelper.cpp Helper classes and functions for SRP
//

// includes
#include "stdafx.h"
#include "SRPHelper.hpp"

using namespace SRP;

BigInteger Helper::GenerateRandomBits(unsigned int uiNumBits)
{
   static boost::xint::strong_random_generator srg;
   return boost::xint::integer::random_by_size(srg,
      uiNumBits,
      false, // high_bit_on
      false, // low_bit_on
      false);// can_be_negative
}

BigInteger Helper::GeneratePassword(
   const std::string& strUsername,
   const std::string& strPassword,
   const GroupParameter& gp,
   const BigInteger& s)
{
   // x = SHA1(s | SHA1(I | ":" | P))
   BigInteger x = Helper::CalcLowerX(strUsername, strPassword, s);

   // v = g^x % N
   return boost::xint::powmod(gp.g, x, gp.N);
}

BigInteger Helper::CalcLowerX(const std::string& strUsername, const std::string& strPassword,
                              const BigInteger& s)
{
   // x = SHA1(s | SHA1(I | ":" | P))
   std::string strUserPass = strUsername + ":" + strPassword;

   BinaryData data1;
   Helper::ToBinaryData(s, data1);

   BinaryData temp1 = Sha1(strUserPass);
   data1.insert(data1.end(), temp1.begin(), temp1.end());

   BinaryData xbin = Sha1(data1);
   return Helper::ToInteger<BigInteger>(xbin);
}

BigInteger Helper::CalcLowerK(const GroupParameter& gp)
{
   // k = SHA1(N | PAD(g))
   BinaryData temp1, temp2;
   Helper::ToBinaryData(gp.N, temp1);
   Helper::ToBinaryData(gp.g, temp2);
   Helper::Pad(temp2, temp1.size());

   BinaryData temp(temp1);
   temp.insert(temp.end(), temp2.begin(), temp2.end());

   BinaryData kbin = Sha1(temp);
   return Helper::ToInteger<BigInteger>(kbin);
}

BigInteger Helper::CalcLowerU(const BigInteger& A, const BigInteger& B,
                              unsigned int uiPadSize)
{
   // u = SHA1(PAD(A) | PAD(B))
   BinaryData temp;
   Helper::ToBinaryData(A, temp);
   Helper::Pad(temp, uiPadSize);

   BinaryData temp2;
   Helper::ToBinaryData(B, temp2);
   Helper::Pad(temp2, uiPadSize);

   temp.insert(temp.end(), temp2.begin(), temp2.end());

   BinaryData ubin = Sha1(temp);
   return Helper::ToInteger<BigInteger>(ubin);
}

BinaryData Helper::CalcM1(const GroupParameter& gp, const std::string& strUsername,
                          const BigInteger& s, const BigInteger& A, const BigInteger& B,
                          const BinaryData& K)
{
   // M = H(H(N) xor H(g), H(I), s, A, B, K)
   HashDataSha1 m;

   // H(N) xor H(g)
   {
      BinaryData temp1, temp2;
      Helper::ToBinaryData(gp.N, temp1);
      Helper::ToBinaryData(gp.g, temp2);

      temp1 = Sha1(temp1);
      temp2 = Sha1(temp2);

      for (size_t i=0, iMax=temp1.size(); i<iMax; i++)
         temp1[i] ^= temp2[i];

      m.Add(temp1);
   }

   // H(I)
   m.Add(Sha1(strUsername));

   // s, A, B, K
   BinaryData temp;
   Helper::ToBinaryData(s, temp);
   m.Add(temp);

   Helper::ToBinaryData(A, temp);
   m.Add(temp);

   Helper::ToBinaryData(B, temp);
   m.Add(temp);

   m.Add(K);

   return m.Finish();
}

BinaryData Helper::CalcM2(BinaryData m1, const BigInteger& A, const BinaryData& K)
{
   // M = H(A, M, K)
   HashDataSha1 m;

   BinaryData temp;
   Helper::ToBinaryData(A, temp);
   m.Add(temp);

   m.Add(m1);

   m.Add(K);

   return m.Finish();
}


#include "sha1.h"

using SRP::Helper::HashDataSha1;

HashDataSha1::HashDataSha1()
:m_spContext(new SHA_CTX)
{
   SHA1_Init(m_spContext.get());
}

void HashDataSha1::Add(const BinaryData& data)
{
   ATLASSERT(data.size() <= std::numeric_limits<unsigned int>::max());
   if (!data.empty())
      SHA1_Update(m_spContext.get(), &data[0], static_cast<unsigned int>(data.size()));
}

BinaryData HashDataSha1::Finish()
{
   BinaryData data(SHA1_DIGEST_LENGTH);
   SHA1_Final(&data[0], m_spContext.get());

   return data;
}

BinaryData Helper::Sha1(const unsigned char* pData, size_t uiLen)
{
   ATLASSERT(uiLen <= std::numeric_limits<unsigned int>::max());

   SHA_CTX ctx;
   SHA1_Init(&ctx);
   SHA1_Update(&ctx, pData, static_cast<unsigned int>(uiLen));

   BinaryData data(SHA1_DIGEST_LENGTH);
   SHA1_Final(&data[0], &ctx);

   return data;
}

BinaryData Helper::Sha1(const BinaryData& data)
{
   ATLASSERT(!data.empty());
   return Sha1(&data[0], data.size());
}

BinaryData Helper::Sha1(const std::string& str)
{
   ATLASSERT(str.size() > 0);
   return Sha1(reinterpret_cast<const unsigned char*>(str.c_str()), str.size());
}
