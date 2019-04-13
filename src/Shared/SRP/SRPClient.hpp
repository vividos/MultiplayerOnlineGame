//
// SRP xint - SRP implementation using boost::xint
// Copyright (C) 2011-2014 Michael Fink
//
/// \file SRPClient.hpp Client class for SRP authentication
//
#pragma once

// includes
#include "SRPCommon.hpp"
#include <string>

namespace SRP
{

/// client side of SRP authentication
class Client
{
public:
   /// ctor
   Client(GroupParameter gp)
      :m_gp(gp)
   {
   }

   /// calculates A from random a
   const BigInteger& GetA(const BigInteger& a);

   /// calculates client session key K and hash Mc
   const BinaryData& CalcM(const std::string& strUsername, const std::string& strPassword,
      const BigInteger& s, const BigInteger& B);

   /// checks if the server knows us
   bool VerifyServer(const BinaryData& Mserver) const;

   /// returns session key K, calculated by CalcM()
   const BinaryData& GetK()
   {
      ATLASSERT(!m_K.empty());
      return m_K;
   }

private:
   GroupParameter m_gp;       ///< group parameter
   BigInteger m_a;  ///< client secret
   BigInteger m_A;  ///< client public value
   BinaryData m_K;   ///< shared secret
   BinaryData m_M1;  ///< client's hash Mc
};

} // namespace SRP
