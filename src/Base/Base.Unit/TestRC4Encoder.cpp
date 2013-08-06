//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file TestRC4Encoder.cpp Unit tests for class RC4::Encoder
//

// includes
#include "stdafx.h"
#include "RC4Encoder.hpp"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace
{

/// test data for encryption
struct STestData
{
   const char* key,* text,* result;
} test_vectors[] = 
{
   {"Key",     "Plaintext",      "BBF316E8D940AF0AD3"},
   {"Wiki",    "pedia",          "1021BF0420"},
   {"Secret",  "Attack at dawn", "45A01F645FC35B383552544B9BF5"}
};

template <typename TForwardIterator>
CString FormatHexData(TForwardIterator itBegin, TForwardIterator itEnd)
{
   CString cszText, cszTemp;

   while (itBegin != itEnd)
   {
      cszTemp.Format(_T("%02X"), *(itBegin++));
      cszText += cszTemp;
   }

   return cszText;
}

} // unnamed namespace

namespace UnitTest
{

/// tests RC4::Encoder class
TEST_CLASS(TestRC4Encoder)
{
   /// tests Get() method of RC4 encoder
   TEST_METHOD(TestGet)
   {
      for (int x = 0; x < sizeof(test_vectors)/sizeof(test_vectors[0]); x++)
      {
         STestData& testData = test_vectors[x];

         const unsigned char* key = reinterpret_cast<const unsigned char*>(testData.key);
         unsigned int key_length = strlen(testData.key);

         RC4::Encoder rc4(key, key_length);

         CString cszResult, cszTemp;

         unsigned int text_length = strlen(testData.text);
         for (size_t y = 0; y < text_length; y++)
         {
            unsigned char bEncrypted = testData.text[y] ^ rc4.Get();
            cszTemp.Format(_T("%02X"), bEncrypted);
            cszResult += cszTemp;
         }

         Assert::IsTrue(CString(testData.result) == cszResult);
      }
   }

   /// tests Encrypt() method of RC4 encoder
   TEST_METHOD(TestEncrypt)
   {
      for (int x = 0; x < sizeof(test_vectors)/sizeof(test_vectors[0]); x++)
      {
         STestData& testData = test_vectors[x];

         std::vector<unsigned char> vecData(testData.text, testData.text+strlen(testData.text));

         RC4::Encoder rc4(reinterpret_cast<const unsigned char*>(testData.key), strlen(testData.key));
         rc4.Encrypt(vecData.begin(), vecData.end());

         Assert::IsTrue(CString(testData.result) == FormatHexData(vecData.begin(), vecData.end()));
      }
   }

   /// tests crypt/decrypt roundtrip
   TEST_METHOD(TestCryptDecrypt)
   {
      for (int x = 0; x < sizeof(test_vectors)/sizeof(test_vectors[0]); x++)
      {
         STestData& testData = test_vectors[x];

         std::vector<unsigned char> vecData(testData.text, testData.text+strlen(testData.text));

         RC4::Encoder rc4encrypt(reinterpret_cast<const unsigned char*>(testData.key), strlen(testData.key));
         rc4encrypt.Encrypt(vecData.begin(), vecData.end());

         RC4::Encoder rc4decrypt(reinterpret_cast<const unsigned char*>(testData.key), strlen(testData.key));

         rc4decrypt.Encrypt(vecData.begin(), vecData.end());

         vecData.push_back(0); // text delimiter

         Assert::IsTrue(CString(testData.text) == CString(&vecData[0]));
      }
   }
};

} // namespace UnitTest
