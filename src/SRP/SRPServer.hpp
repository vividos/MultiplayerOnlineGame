//
// SRP xint - SRP implementation using boost::xint
// Copyright (C) 2011-2013 Michael Fink
//
//! \file SRPServer.hpp Server class for SRP authentication
//
#pragma once

// includes
#include "SRPCommon.hpp"
#include <string>

namespace SRP
{

/// server side of SRP authentication
class Server
{
public:
   /// inits server
   Server(const GroupParameter& gp)
      :m_gp(gp)
   {
   }

   /// calculates B from user's password verifier and server secret b
   const BigInteger& GetB(const BigInteger& PassVerifier, const BigInteger& b);

   /// calculates session key
   void CalcSessionKey(const BigInteger& A, const BigInteger& s, const std::string& strUsername,
      const BigInteger& PassVerifier);

   /// checks client's M value, and when valid, returns own M value
   BinaryData VerifyClient(const BinaryData& Mclient);

   /// returns K
   const BinaryData& GetK() const throw()
   {
      ATLASSERT(!m_K.empty());
      return m_K;
   }

private:
   GroupParameter m_gp;       ///< group parameter
   BigInteger m_b;            ///< server secret
   BigInteger m_B;            ///< server public value
   BigInteger m_A;            ///< client public value
   BigInteger m_s;            ///< salt for this user
   std::string m_strUsername; ///< username
   BinaryData m_K;            ///< shared secret
};

} // namespace SRP
