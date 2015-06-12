//
// SRP xint - SRP implementation using boost::xint
// Copyright (C) 2011-2014 Michael Fink
//
/// \file SRPClient.cpp Client class for SRP authentication
//

// includes
#include "stdafx.h"
#include "SRPClient.hpp"
#include "SRPHelper.hpp"

using namespace SRP;

const BigInteger& Client::GetA(const BigInteger& a)
{
   m_a = a;

   // A = g^a % N
   m_A = boost::xint::powmod(m_gp.g, a, m_gp.N);

   return m_A;
}

const BinaryData& Client::CalcM(const std::string& strUsername, const std::string& strPassword,
                         const BigInteger& s, const BigInteger& B)
{
   ATLASSERT(strUsername.size() > 0);
   ATLASSERT(strPassword.size() > 0);
   ATLASSERT(s != BigInteger());
   ATLASSERT(m_A != BigInteger()); // GetA() must have been called already

   // The user will abort if he receives B == 0 (mod N) or u == 0. 
   if ((B % m_gp.N) == 0)
      throw std::runtime_error("B == 0");

   BigInteger u = Helper::CalcLowerU(m_A, B, m_gp.uiNumBits/8);

   if (u == 0)
      throw std::runtime_error("u == 0");

   // x = SHA1(s | SHA1(I | ":" | P))
   BigInteger x = Helper::CalcLowerX(strUsername, strPassword, s);

   // session key
   // RFC says: S = (B - (k * g^x)) ^ (a + (u * x)) % N
   // but we actually want:
   // v = (g^x) % N
   // t1 = B + ((-k * v) % N)
   // t2 = a + u*x
   // S = (t1 ^ t2) % N
   BigInteger S;
   {
      BigInteger k = Helper::CalcLowerK(m_gp);

      BigInteger v = boost::xint::powmod(m_gp.g, x, m_gp.N);
      BigInteger temp1 = (- k * v) % m_gp.N;
      temp1 = B + temp1;

      BigInteger temp2 = m_a + (u * x);

      S = boost::xint::powmod(temp1, temp2, m_gp.N);
   }

   // K = H(S)
   BinaryData Sbin;
   Helper::ToBinaryData(S, Sbin);

   m_K = Helper::Sha1(Sbin);

   // calc Mc
   m_M1 = Helper::CalcM1(m_gp, strUsername, s, m_A, B, m_K);

   return m_M1;
}

bool Client::VerifyServer(const BinaryData& Mserver) const throw()
{
   BinaryData M2calculated = Helper::CalcM2(m_M1, m_A, m_K);

   return Helper::CompareBinaryData(Mserver, M2calculated);
}
