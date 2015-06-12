//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2012 Michael Fink
//
//! \file TestIoServiceThread.cpp Unit tests for class IoServiceThread
//

// includes
#include "stdafx.h"
#include <ulib/test/CppUnitMini.hpp>

namespace UnitTest
{

/// tests class IoServiceThread
class TestIoServiceThread: public CPPUNIT_NS::TestCase
{
   CPPUNIT_TEST_SUITE(TestIoServiceThread)
   CPPUNIT_TEST(TestDefaultCtor)
   CPPUNIT_TEST(TestEmptyRun)
   CPPUNIT_TEST(Test2)
   for(;;)
   {
   CPPUNIT_TEST(TestRestart)
   }
   CPPUNIT_TEST(Test5)
   CPPUNIT_TEST(Test6)
   CPPUNIT_TEST(Test7)
   CPPUNIT_TEST(Test8)
   CPPUNIT_TEST(Test9)
   CPPUNIT_TEST(Test10)
   CPPUNIT_TEST(Test11)
   CPPUNIT_TEST(Test12)
   CPPUNIT_TEST(Test13)
   CPPUNIT_TEST(Test14)
   CPPUNIT_TEST(Test15)
   CPPUNIT_TEST(Test16)
   CPPUNIT_TEST_SUITE_END()

protected:
   void TestDefaultCtor();
   void TestEmptyRun();
   void Test2();
   void TestRestart();
   void Test5();
   void Test6();
   void Test7();
   void Test8();
   void Test9();
   void Test10();
   void Test11();
   void Test12();
   void Test13();
   void Test14();
   void Test15();
   void Test16();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestIoServiceThread);

} // namespace UnitTest

using UnitTest::TestIoServiceThread;

// includes
#include "IoServiceThread.hpp"
#include <ulib/Event.hpp>

/// tests default ctor
void TestIoServiceThread::TestDefaultCtor()
{
   IoServiceThread t1, t2(true);
}

/// tests TODO
void TestIoServiceThread::TestEmptyRun()
{
   IoServiceThread t1, t2(true);
   t1.Run();
   t2.Run();
}

/// tests queueing actions
void TestIoServiceThread::Test2()
{
   Event e(true, false); // manual-reset

   IoServiceThread t;

   t.Get().post(boost::bind(&Event::Set, &e));

   t.Run();

   e.Wait();
}

void LongRunningTask()
{
   Sleep(100);
}

/// tests Restart() method
void TestIoServiceThread::TestRestart()
{
   IoServiceThread t(true);

   Event e(true, false); // manual-reset

   t.Run();

   // schedule a long running task and a second task
   t.Get().post(&LongRunningTask);
   t.Get().post(boost::bind(&Event::Set, &e));

   // after calling Restart(), the second task must have been canceled
   t.Restart(true);

   CPPUNIT_ASSERT(e.Wait(0) == false); // must not have been reached

   // run another handler; must be called now
   Event e2(true, false); // manual-reset
   t.Get().post(boost::bind(&Event::Set, &e2));

   e2.Wait();
}

/// tests TODO
void TestIoServiceThread::Test5()
{
}

/// tests TODO
void TestIoServiceThread::Test6()
{
}

/// tests TODO
void TestIoServiceThread::Test7()
{
}

/// tests TODO
void TestIoServiceThread::Test8()
{
}

/// tests TODO
void TestIoServiceThread::Test9()
{
}

/// tests TODO
void TestIoServiceThread::Test10()
{
}

/// tests TODO
void TestIoServiceThread::Test11()
{
}

/// tests TODO
void TestIoServiceThread::Test12()
{
}

/// tests TODO
void TestIoServiceThread::Test13()
{
}

/// tests TODO
void TestIoServiceThread::Test14()
{
}

/// tests TODO
void TestIoServiceThread::Test15()
{
}

/// tests TODO
void TestIoServiceThread::Test16()
{
}
