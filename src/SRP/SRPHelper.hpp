//
// SRP xint - SRP implementation using boost::xint
// Copyright (C) 2011-2013 Michael Fink
//
//! \file SRPHelper.hpp Helper classes and functions for SRP
//
#pragma once

// includes
#include "SRPCommon.hpp"
#include <string>

// forward references
struct _SHA_CTX;

namespace SRP
{

/// helper functions
namespace Helper
{
   /// converts to binary representation (high-to-low bytes) and adds to given BinaryData variable
   /// \tparam TIntNumber boost::xint integer type
   template <typename TIntNumber>
   void ToBinaryData(const TIntNumber& n, BinaryData& data)
   {
      BinaryData tempData = boost::xint::to_binary(n);

      // since all operators expect a high byte...low byte order, reverse the data
      data.reserve(data.size() + tempData.size());
      std::copy(tempData.rbegin(), tempData.rend(), std::back_inserter(data));
   }

   /// converts binary data to integer
   /// \tparam TIntNumber boost::xint integer type
   template <typename TIntNumber>
   TIntNumber ToInteger(const BinaryData& data, bool bFromBigEndian = true)
   {
      if (!bFromBigEndian)
         return TIntNumber(data);

      BinaryData tempData;
      tempData.reserve(data.size());

      std::copy(data.rbegin(), data.rend(), std::back_inserter(tempData));

      return TIntNumber(tempData);
   }

   /// converts integer to uppercase hex string representation
   /// \tparam TIntNumber boost::xint integer type
   template <typename TIntNumber>
   std::string ToString(const TIntNumber& n)
   {
      // always hex, and uppercase
      return boost::xint::to_string(n, 16, true);
   }

   /// pad binary representation to given size
   inline void Pad(BinaryData& data, size_t uiTargetSize)
   {
      if (data.size() < uiTargetSize)
      {
         data.reserve(uiTargetSize);
         data.insert(data.begin(), uiTargetSize - data.size(), 0);
      }
   }

   /// compare tow binary data blocks for equality
   inline bool CompareBinaryData(const BinaryData& data1, const BinaryData& data2)
   {
      return data1.size() == data2.size() &&
         0 == memcmp(&data1[0], &data2[0], data1.size());
   }

   /// generates random integer with given bit count
   BigInteger GenerateRandomBits(unsigned int uiNumBits);

   /// generates password verifier value v
   BigInteger GeneratePassword(
      const std::string& strUsername,
      const std::string& strPassword,
      const GroupParameter& gp,
      const BigInteger& s);

   /// calculates x = SHA1(s | SHA1(I | ":" | P))
   BigInteger CalcLowerX(const std::string& strUsername, const std::string& strPassword,
      const BigInteger& s);

   /// calculates k = SHA1(N | PAD(g))
   BigInteger CalcLowerK(const GroupParameter& gp);

   /// calculates u = SHA1(PAD(A) | PAD(B))
   BigInteger CalcLowerU(const BigInteger& A, const BigInteger& B, unsigned int uiPadSize);

   /// calculates client side M = H(H(N) xor H(g), H(I), s, A, B, K)
   BinaryData CalcM1(const GroupParameter& gp, const std::string& strUsername,
      const BigInteger& s, const BigInteger& A, const BigInteger& B, const BinaryData& K);

   /// calculates server side M = H(A, M, K)
   BinaryData CalcM2(BinaryData m1, const BigInteger& A, const BinaryData& K);

   /// returns number of preset group parameters
   unsigned int GetMaxCountPresetGroupParameter() throw();

   /// returns prepared modulus/generator pairs, by index
   GroupParameter GetPresetGroupParameter(unsigned int uiIndex);

   /// class to calculate SHA-1 hash of data
   class HashDataSha1
   {
   public:
      /// ctor
      HashDataSha1() throw();

      /// adds more data
      void Add(const BinaryData& data);

      /// finalizes hash value
      BinaryData Finish();

   private:
      /// context object
      boost::shared_ptr<_SHA_CTX> m_spContext;
   };

   /// calculates SHA-1 hash from data
   BinaryData Sha1(const unsigned char* pData, size_t uiLen);

   /// calculates SHA-1 hash from data
   BinaryData Sha1(const BinaryData& data);

   /// calculates SHA-1 hash from string
   BinaryData Sha1(const std::string& str);

} // namespace Helper

} // namespace SRP
