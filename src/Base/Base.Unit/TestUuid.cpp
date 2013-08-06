//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file TestUuid.cpp Unit tests for class Uuid
//

// includes
#include "stdafx.h"
#include "Uuid.hpp"
#include <set>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static LPCTSTR c_pszNullGuid = _T("00000000-0000-0000-0000-000000000000");
static LPCTSTR c_pszTestGuid = _T("aa7fe05f-a5e2-478f-ad3e-2e53cf4d661c");

namespace UnitTest
{

/// tests class Uuid
TEST_CLASS(TestUuid)
{
   /// tests ctor taking null guid string
   TEST_METHOD(TestCtorGuid1)
   {
      Uuid u(c_pszNullGuid);

      Assert::IsTrue(u == Uuid::Null());
   }

   /// tests ctor taking test guid string
   TEST_METHOD(TestCtorGuid2)
   {
      Uuid u(c_pszTestGuid);
      Assert::AreEqual(u.ToString(), c_pszTestGuid);
   }

   /// tests ctor taking 16 bytes of raw data
   TEST_METHOD(TestCtorRaw)
   {
      std::vector<BYTE> vecData(16, 0);
      Uuid u(&vecData[0]);

      Assert::IsTrue(u == Uuid::Null());
   }

   /// tests Null() function
   TEST_METHOD(TestNullUuid)
   {
      Uuid u = Uuid::Null();
      Assert::AreEqual(u.ToString(), c_pszNullGuid);
   }

   /// tests New() function
   TEST_METHOD(TestNewGuid)
   {
      Uuid u = Uuid::New();
      Assert::IsTrue(u != Uuid::Null());
   }

   /// tests ToString() and FromString()
   TEST_METHOD(TestToStringFromString)
   {
      Uuid u1 = Uuid::New();
      CString cszUuid = u1.ToString();

      Uuid u2 = Uuid::FromString(cszUuid);
      Assert::IsTrue(u1 == u2);
   }

   /// tests equal operator
   TEST_METHOD(TestEqualOperator)
   {
      Uuid u1(c_pszTestGuid);
      Uuid u2(c_pszTestGuid);

      Assert::IsTrue(u1 == u2);
   }

   /// tests unequal operator
   TEST_METHOD(TestUnequalOperator)
   {
      Uuid u1 = Uuid::New();
      Uuid u2 = Uuid::New();

      Assert::IsTrue(u1 != u2);
   }

   /// tests copy ctor
   TEST_METHOD(TestCopyCtor)
   {
      Uuid u1 = Uuid::New();
      Uuid u2(u1);

      Assert::IsTrue(u1 == u2);
   }

   /// tests assignment operator
   TEST_METHOD(TestAssignOperator)
   {
      Uuid u1 = Uuid::New();

      Uuid u2 = u1;
      Assert::IsTrue(u1 == u2);

      u1 = u2;
      Assert::IsTrue(u1 == u2);
   }

   /// tests Raw() method, part 1
   TEST_METHOD(TestRaw1)
   {
      const BYTE abGuid[16] = {
         0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
         0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10
      };

      Uuid u(abGuid);

      const BYTE* pbGuid = u.Raw();

      Assert::IsTrue(0 == memcmp(abGuid, pbGuid, sizeof(abGuid)));
   }

   /// tests Raw() method, part 2
   TEST_METHOD(TestRaw2)
   {
      Uuid u1 = Uuid::New();

      Uuid u2(u1.Raw());

      Assert::IsTrue(u1 == u2);
   }

   /// tests less-than operator
   TEST_METHOD(TestLessOperator)
   {
      std::set<Uuid> setUuids;

      Uuid u1 = Uuid::New();
      Uuid u2 = Uuid::New();

      setUuids.insert(u1);
      setUuids.insert(u2);

      Assert::IsTrue(setUuids.find(u1) != setUuids.end());
      Assert::IsTrue(setUuids.find(u2) != setUuids.end());

      Uuid u3 = Uuid::New();
      Assert::IsTrue(setUuids.find(u3) == setUuids.end());
   }
};

} // namespace UnitTest
