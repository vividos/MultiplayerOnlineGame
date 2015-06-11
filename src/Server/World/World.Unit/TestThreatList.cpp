//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestThreatList.cpp Unit tests for class ThreatList
//

// includes
#include "stdafx.h"
#include "Object.hpp"
#include "ThreatList.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{

const signed int c_uiPositiveThreat = 100;
const signed int c_uiNegativeThreatBig = -125;
const signed int c_uiNegativeThreatSmall = -75;

/// tests class ThreatList
TEST_CLASS(TestThreatList)
{
public:
   TestThreatList()
      :m_id(ObjectId::New()),
       m_id2(ObjectId::New())
   {
   }

   /// tests default ctor
   TEST_METHOD(TestDefaultCtor)
   {
      ThreatList t1, t2;
   }

   /// tests Add() function, new entry
   TEST_METHOD(TestAddNewEntry)
   {
      ThreatList t;

      ThreatList::T_enThreatListOutcome enOutcome =
         t.Add(m_id, c_uiPositiveThreat);

      Assert::IsTrue(enOutcome == ThreatList::threatListAdded);
   }

   /// tests Add() function, updating entry
   TEST_METHOD(TestAddUpdateEntry)
   {
      ThreatList t;

      ThreatList::T_enThreatListOutcome enOutcome =
         t.Add(m_id, c_uiPositiveThreat);
      Assert::IsTrue(enOutcome == ThreatList::threatListAdded);

      enOutcome = t.Add(m_id, c_uiPositiveThreat);
      Assert::IsTrue(enOutcome == ThreatList::threatListNothing);
   }

   /// tests Add() function, removing entry again
   TEST_METHOD(TestAddRemoveByAdd)
   {
      ThreatList t;

      ThreatList::T_enThreatListOutcome enOutcome =
         t.Add(m_id, c_uiPositiveThreat);
      Assert::IsTrue(enOutcome == ThreatList::threatListAdded);

      enOutcome = t.Add(m_id, c_uiNegativeThreatBig);
      Assert::IsTrue(enOutcome == ThreatList::threatListRemoved);
   }

   /// tests Add() function, not removing entry again
   TEST_METHOD(TestAddNotRemoveEntry)
   {
      ThreatList t;

      ThreatList::T_enThreatListOutcome enOutcome =
         t.Add(m_id, c_uiPositiveThreat);
      Assert::IsTrue(enOutcome == ThreatList::threatListAdded);

      enOutcome = t.Add(m_id, c_uiNegativeThreatSmall);
      Assert::IsTrue(enOutcome == ThreatList::threatListNothing);
   }

   /// tests Add() function, removing entry and then trying to remove again
   TEST_METHOD(TestAddRemoveRemoveAgain)
   {
      ThreatList t;

      ThreatList::T_enThreatListOutcome enOutcome =
         t.Add(m_id, c_uiPositiveThreat);
      Assert::IsTrue(enOutcome == ThreatList::threatListAdded);

      enOutcome = t.Add(m_id, c_uiNegativeThreatBig);
      Assert::IsTrue(enOutcome == ThreatList::threatListRemoved);

      enOutcome = t.Add(m_id, c_uiNegativeThreatSmall);
      Assert::IsTrue(enOutcome == ThreatList::threatListNothing);
   }

   /// tests IsInList() function with Add() and removing during second Add() call
   TEST_METHOD(TestAddRemoveByAddNegative)
   {
      ThreatList t;

      Assert::IsTrue(false == t.IsInList(m_id));

      t.Add(m_id, c_uiPositiveThreat);
      Assert::IsTrue(true == t.IsInList(m_id));

      t.Add(m_id, c_uiNegativeThreatBig);
      Assert::IsTrue(false == t.IsInList(m_id));
   }

   /// tests IsInList() function with Add() and Remove() call
   TEST_METHOD(TestIsInListAddRemoveOneEntry)
   {
      ThreatList t;

      Assert::IsTrue(false == t.IsInList(m_id));

      t.Add(m_id, c_uiPositiveThreat);
      Assert::IsTrue(true == t.IsInList(m_id));

      t.Remove(m_id);
      Assert::IsTrue(false == t.IsInList(m_id));
   }

   /// tests IsInList() function with Add() and Remove() call
   TEST_METHOD(TestIsInListAddRemoveTwoEntries)
   {
      ThreatList t;

      Assert::IsTrue(false == t.IsInList(m_id));
      Assert::IsTrue(false == t.IsInList(m_id2));

      t.Add(m_id, c_uiPositiveThreat);
      Assert::IsTrue(true == t.IsInList(m_id));
      Assert::IsTrue(false == t.IsInList(m_id2));

      t.Add(m_id2, c_uiPositiveThreat);
      Assert::IsTrue(true == t.IsInList(m_id));
      Assert::IsTrue(true == t.IsInList(m_id2));

      t.Remove(m_id);
      Assert::IsTrue(false == t.IsInList(m_id));
      Assert::IsTrue(true == t.IsInList(m_id2));
   }

   /// tests IsInList() function with Add() and RemoveAll() call
   TEST_METHOD(TestIsInListAddRemoveAll)
   {
      ThreatList t;

      Assert::IsTrue(false == t.IsInList(m_id));
      Assert::IsTrue(false == t.IsInList(m_id2));

      t.Add(m_id, c_uiPositiveThreat);
      Assert::IsTrue(true == t.IsInList(m_id));
      Assert::IsTrue(false == t.IsInList(m_id2));

      t.Add(m_id2, c_uiPositiveThreat);
      Assert::IsTrue(true == t.IsInList(m_id));
      Assert::IsTrue(true == t.IsInList(m_id2));

      t.RemoveAll();

      Assert::IsTrue(false == t.IsInList(m_id));
      Assert::IsTrue(false == t.IsInList(m_id2));
   }

   /// tests Add() function, not changing order
   TEST_METHOD(TestAddAddNoReorder)
   {
      ThreatList t;

      t.Add(m_id, c_uiPositiveThreat);
      ThreatList::T_enThreatListOutcome enOutcome =
         t.Add(m_id2, c_uiPositiveThreat-10);

      Assert::IsTrue(enOutcome == ThreatList::threatListAdded);
   }

   /// tests Add() function, changing order
   TEST_METHOD(TestAddAddReorder)
   {
      ThreatList t;

      t.Add(m_id, c_uiPositiveThreat);
      ThreatList::T_enThreatListOutcome enOutcome =
         t.Add(m_id2, c_uiPositiveThreat+10);

      Assert::IsTrue(enOutcome == ThreatList::threatListAdded);
   }

   /// tests Add() function, changing order by update, second is moved to first place
   TEST_METHOD(TestAddReorderFirstToSecond)
   {
      ThreatList t;

      t.Add(m_id, c_uiPositiveThreat);
      t.Add(m_id2, c_uiPositiveThreat+10);
      ThreatList::T_enThreatListOutcome enOutcome = t.Add(m_id, 11);

      Assert::IsTrue(enOutcome == ThreatList::threatListOrderChanged);
   }

   /// tests Add() function, changing order by update, first is moved to second place
   TEST_METHOD(TestAddReorderSecondToFirst)
   {
      ThreatList t;

      t.Add(m_id, c_uiPositiveThreat);
      t.Add(m_id2, c_uiPositiveThreat+10);
      ThreatList::T_enThreatListOutcome enOutcome = t.Add(m_id2, -11);

      Assert::IsTrue(enOutcome == ThreatList::threatListOrderChanged);
   }

   /// tests Top() function, not changing order
   TEST_METHOD(TestTopNoReorder)
   {
      ThreatList t;

      t.Add(m_id, c_uiPositiveThreat);
      Assert::IsTrue(t.Top() == m_id);

      t.Add(m_id2, c_uiPositiveThreat-10);
      Assert::IsTrue(t.Top() == m_id);
   }

   /// tests Top() function, changing order
   TEST_METHOD(TestTopReorder)
   {
      ThreatList t;

      t.Add(m_id, c_uiPositiveThreat);
      Assert::IsTrue(t.Top() == m_id);

      t.Add(m_id2, c_uiPositiveThreat+10);
      Assert::IsTrue(t.Top() == m_id2);
   }

   /// tests Top() function, ordering of two entries with same threat
   TEST_METHOD(TestTopReorderSameThreat)
   {
      ThreatList t;

      t.Add(m_id, c_uiPositiveThreat);
      Assert::IsTrue(t.Top() == m_id);

      t.Add(m_id2, c_uiPositiveThreat);
      Assert::IsTrue(t.Top() == m_id2); // last one should win
   }


private:
   ObjectId m_id;
   ObjectId m_id2;
};

} // namespace UnitTest
