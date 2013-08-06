//
// ulib - a collection of useful classes
// Copyright (C) 2006,2007,2008,2009,2012 Michael Fink
//
/// \file TestDateTime.cpp tests for DateTime class
//

// includes
#include "stdafx.h"
#include <ulib/DateTime.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
const double g_dEpsilon = 1e-6;

/// tests DateTime and TimeSpan classes
TEST_CLASS(TestDateTime)
{
   /// tests default ctor
   TEST_METHOD(TestCtorDefault)
   {
      DateTime dt;
      Assert::IsTrue(DateTime::invalid == dt.Status());
   }

   /// tests ctor with data
   TEST_METHOD(TestCtorDateTime)
   {
      // ctor
      DateTime dt(2007, 5, 19, 1, 40, 26, 952);
      Assert::IsTrue(DateTime::valid == dt.Status());

      Assert::IsTrue(2007 == dt.Year());
      Assert::IsTrue(5 == dt.Month());
      Assert::IsTrue(19 == dt.Day());

      Assert::IsTrue(1 == dt.Hour());
      Assert::IsTrue(40 == dt.Minute());
      Assert::IsTrue(26 == dt.Second());
      Assert::IsTrue(952 == dt.Millisecond());

      Assert::IsTrue(6 == dt.DayOfWeek()); // 6: saturday
      Assert::IsTrue(139 == dt.DayOfYear());
   }

   /// tests ctor with min/max status
   TEST_METHOD(TestCtorStatus)
   {
      DateTime dt1(DateTime::min), dt2(DateTime::max);
      Assert::IsTrue(DateTime::min == dt1.Status());
      Assert::IsTrue(DateTime::max == dt2.Status());
   }

   /// tests copy ctor
   TEST_METHOD(TestCtorCopy)
   {
      DateTime dt1(2007, 5, 19, 1, 40, 26, 952);
      Assert::IsTrue(DateTime::valid == dt1.Status());

      DateTime dt2(dt1);
      Assert::IsTrue(DateTime::valid == dt2.Status());

      Assert::IsTrue(2007 == dt2.Year());
      Assert::IsTrue(5 == dt2.Month());
      Assert::IsTrue(19 == dt2.Day());

      Assert::IsTrue(1 == dt2.Hour());
      Assert::IsTrue(40 == dt2.Minute());
      Assert::IsTrue(26 == dt2.Second());

      Assert::IsTrue(952 == dt2.Millisecond());

      Assert::IsTrue(6 == dt2.DayOfWeek()); // 6: saturday
      Assert::IsTrue(139 == dt2.DayOfYear());

      // check if copy on write doesn't store both values
      dt2.SetDateTime(2006, 5, 19, 1, 40, 26, 953);

      Assert::IsTrue(2007 == dt1.Year());
      Assert::IsTrue(2006 == dt2.Year());
   }

   /// tests assignment operator
   TEST_METHOD(TestOperatorAssignment)
   {
      // ctor
      DateTime dt1(2007, 5, 19, 1, 40, 26, 952), dt2;
      Assert::IsTrue(DateTime::valid == dt1.Status());
      Assert::IsTrue(DateTime::invalid == dt2.Status());

      dt2 = dt1;
      Assert::IsTrue(DateTime::valid == dt2.Status());

      Assert::IsTrue(2007 == dt2.Year());
      Assert::IsTrue(5 == dt2.Month());
      Assert::IsTrue(19 == dt2.Day());

      Assert::IsTrue(1 == dt2.Hour());
      Assert::IsTrue(40 == dt2.Minute());
      Assert::IsTrue(26 == dt2.Second());

      Assert::IsTrue(952 == dt2.Millisecond());

      Assert::IsTrue(6 == dt2.DayOfWeek()); // 6: saturday
      Assert::IsTrue(139 == dt2.DayOfYear());

      // check if copy on write doesn't store both values
      dt2.SetDateTime(2006, 5, 19, 1, 40, 26, 952);

      Assert::IsTrue(2007 == dt1.Year());
      Assert::IsTrue(2006 == dt2.Year());
   }

   /// tests current date
   TEST_METHOD(TestNowToday)
   {
      DateTime dtNow = DateTime::Now();
      DateTime dtToday = DateTime::Today();
      Assert::IsTrue(DateTime::valid == dtNow.Status());
      Assert::IsTrue(DateTime::valid == dtToday.Status());

      Assert::IsTrue(dtNow - dtToday == dtNow.TimeOfDay());
   }

   /// tests operator+
   TEST_METHOD(TestOperatorAdd)
   {
      DateTime dt1(2007, 5, 19, 1, 40, 26, 952);
      TimeSpan ts(23, 21, 35, 47);

      DateTime dt2 = dt1 + ts;

      Assert::IsTrue(2007 == dt2.Year());
      Assert::IsTrue(5 == dt2.Month());
      Assert::IsTrue(20 == dt2.Day());

      Assert::IsTrue((1+23+1)-24 == dt2.Hour());
      Assert::IsTrue((40+21+1)-60 == dt2.Minute());
      Assert::IsTrue((26+35)-60 == dt2.Second());
      Assert::IsTrue(952+47 == dt2.Millisecond());
   }

   /// tests operator- with DateTime
   TEST_METHOD(TestOperatorSubDateTime)
   {
      DateTime dt1(2007, 5, 19, 1, 40, 26, 952);
      DateTime dt2(2007, 5, 18, 18, 55, 31, 0);

      TimeSpan ts1 = dt1 - dt2;
      TimeSpan ts2 = dt2 - dt1;

      Assert::IsTrue(ts1 == -ts2); // also checking unary minus here

      Assert::IsTrue(6 == ts1.Hours());
      Assert::IsTrue(44 == ts1.Minutes());
      Assert::IsTrue(55 == ts1.Seconds());
      Assert::IsTrue(952 == ts1.Milliseconds());
   }

   /// tests operator- with TimeSpan
   TEST_METHOD(TestOperatorSubSpan)
   {
      DateTime dt1(2007, 5, 19, 1, 40, 26, 952);
      TimeSpan ts(6, 44, 55, 47);

      DateTime dt2 = dt1 - ts;

      Assert::IsTrue(DateTime(2007, 5, 18, 18, 55, 31, 905) == dt2);
   }

   /// tests operator+=
   TEST_METHOD(TestOperatorCompoundAdd)
   {
      DateTime dt(2007, 5, 19, 1, 40, 26, 952);
      TimeSpan ts(23, 21, 35, 47);

      dt += ts;

      Assert::IsTrue(DateTime(2007, 5, 20, 1, 2, 1, 999) == dt);
   }

   /// tests operator-=
   TEST_METHOD(TestOperatorCompoundSub)
   {
      DateTime dt(2007, 5, 19, 1, 40, 26, 952);
      TimeSpan ts(6, 44, 55, 47);

      dt -= ts;

      Assert::IsTrue(DateTime(2007, 5, 18, 18, 55, 31, 905) == dt);
   }

   /// tests comparison operators
   TEST_METHOD(TestOperatorsCompare)
   {
      // dt2 is the same as dt1, dt3 is later as dt1
      DateTime dt1(2007, 5, 19, 1, 40, 26, 952), dt2(dt1), dt3(2008, 1, 1, 0, 0, 0, 0);

      // equal
      Assert::IsTrue(dt1 == dt1);
      Assert::IsTrue(dt2 == dt2);
      Assert::IsTrue(dt3 == dt3);
      Assert::IsTrue(dt1 == dt2);
      Assert::IsTrue(dt2 == dt1);
      Assert::IsTrue(!(dt1 == dt3));
      Assert::IsTrue(!(dt2 == dt3));
      Assert::IsTrue(!(dt3 == dt1));
      Assert::IsTrue(!(dt3 == dt2));

      // not equal
      Assert::IsTrue(dt1 != dt3);
      Assert::IsTrue(dt2 != dt3);
      Assert::IsTrue(dt3 != dt1);
      Assert::IsTrue(dt3 != dt2);
      Assert::IsTrue(!(dt1 != dt1));
      Assert::IsTrue(!(dt2 != dt2));
      Assert::IsTrue(!(dt3 != dt3));
      Assert::IsTrue(!(dt1 != dt2));
      Assert::IsTrue(!(dt2 != dt1));

      // less
      Assert::IsTrue(dt1 < dt3);
      Assert::IsTrue(dt2 < dt3);
      Assert::IsTrue(!(dt1 < dt1));
      Assert::IsTrue(!(dt2 < dt2));
      Assert::IsTrue(!(dt3 < dt3));
      Assert::IsTrue(!(dt1 < dt2));
      Assert::IsTrue(!(dt2 < dt1));
      Assert::IsTrue(!(dt3 < dt1));
      Assert::IsTrue(!(dt3 < dt2));

      // greater
      Assert::IsTrue(dt3 > dt1);
      Assert::IsTrue(dt3 > dt2);
      Assert::IsTrue(!(dt1 > dt1));
      Assert::IsTrue(!(dt2 > dt2));
      Assert::IsTrue(!(dt3 > dt3));
      Assert::IsTrue(!(dt1 > dt2));
      Assert::IsTrue(!(dt2 > dt1));
      Assert::IsTrue(!(dt1 > dt3));
      Assert::IsTrue(!(dt2 > dt3));

      // less than or equal
      Assert::IsTrue(dt1 <= dt1);
      Assert::IsTrue(dt2 <= dt2);
      Assert::IsTrue(dt3 <= dt3);
      Assert::IsTrue(dt1 <= dt2);
      Assert::IsTrue(dt2 <= dt1);
      Assert::IsTrue(dt1 <= dt3);
      Assert::IsTrue(dt2 <= dt3);
      Assert::IsTrue(!(dt3 <= dt1));
      Assert::IsTrue(!(dt3 <= dt2));

      // greater than or equal
      Assert::IsTrue(dt1 >= dt1);
      Assert::IsTrue(dt2 >= dt2);
      Assert::IsTrue(dt3 >= dt3);
      Assert::IsTrue(dt1 >= dt2);
      Assert::IsTrue(dt2 >= dt1);
      Assert::IsTrue(dt3 >= dt1);
      Assert::IsTrue(dt3 >= dt2);
      Assert::IsTrue(!(dt1 >= dt3));
      Assert::IsTrue(!(dt2 >= dt3));
   }

   /// tests setting illegal date/time values
   TEST_METHOD(TestSetIllegalDateTime)
   {
      DateTime dt;
      Assert::IsTrue(DateTime::invalid == dt.Status());
   }

   /// tests formatting ISO 8601 dates, part 1
   TEST_METHOD(TestFormatISO8601Part1)
   {
      DateTime dt(2007, 5, 19, 1, 40, 26, 952);

      TimeZone tzUTC = TimeZone::UTC();

      Assert::IsTrue(_T("2007") == dt.FormatISO8601(DateTime::formatY, false, tzUTC));

      Assert::IsTrue(_T("200705") == dt.FormatISO8601(DateTime::formatYM, true, tzUTC));
      Assert::IsTrue(_T("2007-05") == dt.FormatISO8601(DateTime::formatYM, false, tzUTC));

      Assert::IsTrue(_T("20070519") == dt.FormatISO8601(DateTime::formatYMD, true, tzUTC));
      Assert::IsTrue(_T("2007-05-19") == dt.FormatISO8601(DateTime::formatYMD, false, tzUTC));

      Assert::IsTrue(_T("20070519T0140Z") == dt.FormatISO8601(DateTime::formatYMD_HM_Z, true, tzUTC));
      Assert::IsTrue(_T("2007-05-19T01:40Z") == dt.FormatISO8601(DateTime::formatYMD_HM_Z, false, tzUTC));

      Assert::IsTrue(_T("20070519T014026Z") == dt.FormatISO8601(DateTime::formatYMD_HMS_Z, true, tzUTC));
      Assert::IsTrue(_T("2007-05-19T01:40:26Z") == dt.FormatISO8601(DateTime::formatYMD_HMS_Z, false, tzUTC));

      Assert::IsTrue(_T("20070519T014026.952Z") == dt.FormatISO8601(DateTime::formatYMD_HMSF_Z, true, tzUTC));
      Assert::IsTrue(_T("2007-05-19T01:40:26.952Z") == dt.FormatISO8601(DateTime::formatYMD_HMSF_Z, false, tzUTC));
   }

   TEST_METHOD(TestFormatISO8601Part2)
   {
      TimeZone tzSys = TimeZone::System();

      // daylight savings time
      DateTime dt1(2007, 5, 19, 1, 40, 26, 952);

      Assert::IsTrue(_T("20070519T034026.952+0200") == dt1.FormatISO8601(DateTime::formatYMD_HMSF_Z, true, tzSys));
      Assert::IsTrue(_T("2007-05-19T03:40:26.952+02:00") == dt1.FormatISO8601(DateTime::formatYMD_HMSF_Z, false, tzSys));

      // standard savings time
      DateTime dt2(2007, 1, 19, 1, 40, 26, 952);

      Assert::IsTrue(_T("20070119T024026.952+0100") == dt2.FormatISO8601(DateTime::formatYMD_HMSF_Z, true, tzSys));
      Assert::IsTrue(_T("2007-01-19T02:40:26.952+01:00") == dt2.FormatISO8601(DateTime::formatYMD_HMSF_Z, false, tzSys));
   }

   /// tests parsing ISO 8601 dates
   TEST_METHOD(TestParseISO8601)
   {
      DateTime dt1, dt2;

      dt1.ParseISO8601(_T("2010-05-01T18:00:00.000Z"));
      ATLTRACE(dt1.FormatISO8601(DateTime::formatYMD_HMS_Z, false, TimeZone::UTC()));

      dt2.ParseISO8601(_T("2010-05-01T11:00:00.000-07:00"));
      ATLTRACE(dt2.FormatISO8601(DateTime::formatYMD_HMS_Z, false, TimeZone::UTC()));

      Assert::IsTrue(dt1 == dt2);
   }

   /// tests TimeSpan default ctor
   TEST_METHOD(TestSpanCtorDefault)
   {
      TimeSpan ts;
      Assert::IsTrue(TimeSpan::invalid == ts.Status());
   }

   /// tests TimeSpan ctor with time span values, part 1
   TEST_METHOD(TestSpanCtorTimeSpan1)
   {
      // ctor
      TimeSpan ts(1, 40, 26, 123);
      Assert::IsTrue(DateTime::valid == ts.Status());

      Assert::IsTrue(1 == ts.Hours());
      Assert::IsTrue(40 == ts.Minutes());
      Assert::IsTrue(26 == ts.Seconds());
      Assert::IsTrue(123 == ts.Milliseconds());

      Assert::AreEqual(ts.TotalHours(), 1.0, g_dEpsilon);
      Assert::AreEqual(ts.TotalMinutes(),  1.0 * 60 + 40.0, g_dEpsilon);
      Assert::AreEqual(ts.TotalSeconds(), (1.0 * 60 + 40.0) * 60 + 26.0, g_dEpsilon);
      Assert::AreEqual(ts.TotalMilliseconds(), ((1.0 * 60 + 40.0) * 60 + 26.0) * 1000.0 + 123.0, g_dEpsilon);
   }

   /// tests TimeSpan ctor with time span values, part 2
   TEST_METHOD(TestSpanCtorTimeSpan2)
   {
      // ctor
      TimeSpan ts(-4, 10, 2, 123);
      Assert::IsTrue(DateTime::valid == ts.Status());

      Assert::IsTrue(-4 == ts.Hours());
      Assert::IsTrue(-10 == ts.Minutes());
      Assert::IsTrue(-2 == ts.Seconds());
      Assert::IsTrue(-123 == ts.Milliseconds());

      Assert::AreEqual(ts.TotalHours(), -4.0, g_dEpsilon);
      Assert::AreEqual(ts.TotalMinutes(),  -4.0 * 60 + -10.0, g_dEpsilon);
      Assert::AreEqual(ts.TotalSeconds(), (-4.0 * 60 + -10.0) * 60 + -2.0, g_dEpsilon);
      Assert::AreEqual(ts.TotalMilliseconds(), ((-4.0 * 60 + -10.0) * 60 + -2.0) * 1000.0 + -123.0, g_dEpsilon);
   }

   /// tests TimeSpan ctor with time span values, part 3
   TEST_METHOD(TestSpanCtorTimeSpan3)
   {
      // ctor
      TimeSpan ts(1, -40, 26, -123);
      Assert::IsTrue(DateTime::valid == ts.Status());

      Assert::IsTrue(-1 == ts.Hours());
      Assert::IsTrue(-40 == ts.Minutes());
      Assert::IsTrue(-26 == ts.Seconds());
      Assert::IsTrue(-123 == ts.Milliseconds());

      Assert::AreEqual(ts.TotalHours(), -1.0, g_dEpsilon);
      Assert::AreEqual(ts.TotalMinutes(),  -1.0 * 60 + -40.0, g_dEpsilon);
      Assert::AreEqual(ts.TotalSeconds(), (-1.0 * 60 + -40.0) * 60 + -26.0, g_dEpsilon);
      Assert::AreEqual(ts.TotalMilliseconds(), ((-1.0 * 60 + -40.0) * 60 + -26.0) * 1000.0 + -123.0, g_dEpsilon);
   }

   /// tests TimeSpan ctor with time span values, part 4
   TEST_METHOD(TestSpanCtorTimeSpan4)
   {
      // ctor
      TimeSpan ts(-4, -10, 2, 123);
      Assert::IsTrue(DateTime::valid == ts.Status());

      Assert::IsTrue(-4 == ts.Hours());
      Assert::IsTrue(-10 == ts.Minutes());
      Assert::IsTrue(-2 == ts.Seconds());
      Assert::IsTrue(-123 == ts.Milliseconds());

      Assert::AreEqual(ts.TotalHours(), -4.0, g_dEpsilon);
      Assert::AreEqual(ts.TotalMinutes(),  -4.0 * 60 + -10.0, g_dEpsilon);
      Assert::AreEqual(ts.TotalSeconds(), (-4.0 * 60 + -10.0) * 60 + -2.0, g_dEpsilon);
      Assert::AreEqual(ts.TotalMilliseconds(), ((-4.0 * 60 + -10.0) * 60 + -2.0) * 1000.0 + -123.0, g_dEpsilon);
   }

   /// tests TimeSpan ctor with status enum value
   TEST_METHOD(TestSpanCtorStatus)
   {
      TimeSpan ts1(TimeSpan::min), ts2(TimeSpan::max);
      Assert::IsTrue(TimeSpan::min == ts1.Status());
      Assert::IsTrue(TimeSpan::max == ts2.Status());
   }

   /// tests TimeSpan copy ctor
   TEST_METHOD(TestSpanCtorCopy)
   {
      TimeSpan ts1(1, 40, 26, 123);
      Assert::IsTrue(DateTime::valid == ts1.Status());

      TimeSpan ts2(ts1);
      Assert::IsTrue(TimeSpan::valid == ts2.Status());

      Assert::IsTrue(1 == ts2.Hours());
      Assert::IsTrue(40 == ts2.Minutes());
      Assert::IsTrue(26 == ts2.Seconds());
      Assert::IsTrue(123 == ts2.Milliseconds());

      Assert::AreEqual(ts2.TotalHours(), 1.0, g_dEpsilon);
      Assert::AreEqual(ts2.TotalMinutes(),  1.0 * 60 + 40.0, g_dEpsilon);
      Assert::AreEqual(ts2.TotalSeconds(), (1.0 * 60 + 40.0) * 60 + 26.0, g_dEpsilon);
      Assert::AreEqual(ts2.TotalMilliseconds(), ((1.0 * 60 + 40.0) * 60 + 26.0) * 1000.0 + 123.0, g_dEpsilon);

      // check if copy on write doesn't store both values
      ts2.SetDateTimeSpan(2, 20, 13, 124);

      Assert::IsTrue(1 == ts1.Hours());
      Assert::IsTrue(2 == ts2.Hours());
   }

   /// tests TimeSpan assign operator
   TEST_METHOD(TestSpanOperatorAssign)
   {
      // ctor
      TimeSpan ts1(1, 40, 26, 123), ts2;
      Assert::IsTrue(DateTime::valid == ts1.Status());
      Assert::IsTrue(DateTime::invalid == ts2.Status());

      ts2 = ts1;
      Assert::IsTrue(DateTime::valid == ts2.Status());

      Assert::IsTrue(1 == ts2.Hours());
      Assert::IsTrue(40 == ts2.Minutes());
      Assert::IsTrue(26 == ts2.Seconds());
      Assert::IsTrue(123 == ts2.Milliseconds());

      Assert::AreEqual(ts2.TotalHours(), 1.0, g_dEpsilon);
      Assert::AreEqual(ts2.TotalMinutes(),  1.0 * 60 + 40.0, g_dEpsilon);
      Assert::AreEqual(ts2.TotalSeconds(), (1.0 * 60 + 40.0) * 60 + 26.0, g_dEpsilon);
      Assert::AreEqual(ts2.TotalMilliseconds(), ((1.0 * 60 + 40.0) * 60 + 26.0) * 1000.0 + 123.0, g_dEpsilon);

      // check if copy on write doesn't store both values
      ts2.SetDateTimeSpan(2, 20, 13, 124);

      Assert::IsTrue(1 == ts1.Hours());
      Assert::IsTrue(2 == ts2.Hours());
   }

   /// tests TimeSpan operator+
   TEST_METHOD(TestSpanOperatorAdd)
   {
      TimeSpan ts1(1, 40, 26, 123);
      TimeSpan ts2(2, 21, 35, 456);

      Assert::IsTrue(ts1 + ts2 == TimeSpan(4, 2, 1, 579));
   }

   /// tests TimeSpan operator-
   TEST_METHOD(TestSpanOperatorSub)
   {
      TimeSpan ts1(2, 21, 35, 456);
      TimeSpan ts2(1, 40, 26, 123);

      Assert::IsTrue(ts1 - ts2 == TimeSpan(0, 41, 9, 333));
   }

   /// tests TimeSpan operator*
   TEST_METHOD(TestSpanOperatorMultiply)
   {
      TimeSpan ts(1, 40, 26, 123);

      Assert::IsTrue(ts * 3 == TimeSpan(5, 0, 78, 369));
   }

   /// tests TimeSpan operator/
   TEST_METHOD(TestSpanOperatorDivide)
   {
      TimeSpan ts(5, 0, 78, 369);

      Assert::IsTrue(ts / 3 == TimeSpan(1, 40, 26, 123));
   }

   /// tests TimeSpan operator+=
   TEST_METHOD(TestSpanOperatorCompoundAdd)
   {
      TimeSpan ts1(1, 40, 26, 123);
      TimeSpan ts2(2, 21, 35, 456);

      ts1 += ts2;

      Assert::IsTrue(ts1 == TimeSpan(4, 2, 1, 579));
   }

   /// tests TimeSpan operator-=
   TEST_METHOD(TestSpanOperatorCompoundSub)
   {
      TimeSpan ts1(2, 21, 35, 456);
      TimeSpan ts2(1, 40, 26, 123);

      ts1 -= ts2;
      Assert::IsTrue(ts1 == TimeSpan(0, 41, 9, 333));
   }

   /// tests TimeSpan operator*=
   TEST_METHOD(TestSpanOperatorCompoundMultiply)
   {
      TimeSpan ts(1, 40, 26, 123);

      ts *= 3;
      Assert::IsTrue(ts == TimeSpan(5, 0, 78, 369));
   }

   /// tests TimeSpan operator/=
   TEST_METHOD(TestSpanOperatorCompoundDivide)
   {
      TimeSpan ts(5, 0, 78, 369);

      ts /= 3;
      Assert::IsTrue(ts == TimeSpan(1, 40, 26, 123));
   }

   /// tests TimeSpan unary operator-
   TEST_METHOD(TestSpanOperatorUnaryMinus)
   {
      TimeSpan ts1(6, 44, 55, 47);
      TimeSpan ts2 = -ts1;

      Assert::IsTrue(TimeSpan(-6, -44, -55, -47) == ts2);
   }

   /// tests TimeSpan compare operators
   TEST_METHOD(TestSpanOperatorsCompare)
   {
      // ts2 is the same as ts1, ts3 is later as ts1
      TimeSpan ts1(1, 40, 26, 123), ts2(ts1), ts3(2, 1, 27, 456);

      // equal
      Assert::IsTrue(ts1 == ts1);
      Assert::IsTrue(ts2 == ts2);
      Assert::IsTrue(ts3 == ts3);
      Assert::IsTrue(ts1 == ts2);
      Assert::IsTrue(ts2 == ts1);
      Assert::IsTrue(!(ts1 == ts3));
      Assert::IsTrue(!(ts2 == ts3));
      Assert::IsTrue(!(ts3 == ts1));
      Assert::IsTrue(!(ts3 == ts2));

      // not equal
      Assert::IsTrue(ts1 != ts3);
      Assert::IsTrue(ts2 != ts3);
      Assert::IsTrue(ts3 != ts1);
      Assert::IsTrue(ts3 != ts2);
      Assert::IsTrue(!(ts1 != ts1));
      Assert::IsTrue(!(ts2 != ts2));
      Assert::IsTrue(!(ts3 != ts3));
      Assert::IsTrue(!(ts1 != ts2));
      Assert::IsTrue(!(ts2 != ts1));

      // less
      Assert::IsTrue(ts1 < ts3);
      Assert::IsTrue(ts2 < ts3);
      Assert::IsTrue(!(ts1 < ts1));
      Assert::IsTrue(!(ts2 < ts2));
      Assert::IsTrue(!(ts3 < ts3));
      Assert::IsTrue(!(ts1 < ts2));
      Assert::IsTrue(!(ts2 < ts1));
      Assert::IsTrue(!(ts3 < ts1));
      Assert::IsTrue(!(ts3 < ts2));

      // greater
      Assert::IsTrue(ts3 > ts1);
      Assert::IsTrue(ts3 > ts2);
      Assert::IsTrue(!(ts1 > ts1));
      Assert::IsTrue(!(ts2 > ts2));
      Assert::IsTrue(!(ts3 > ts3));
      Assert::IsTrue(!(ts1 > ts2));
      Assert::IsTrue(!(ts2 > ts1));
      Assert::IsTrue(!(ts1 > ts3));
      Assert::IsTrue(!(ts2 > ts3));

      // less than or equal
      Assert::IsTrue(ts1 <= ts1);
      Assert::IsTrue(ts2 <= ts2);
      Assert::IsTrue(ts3 <= ts3);
      Assert::IsTrue(ts1 <= ts2);
      Assert::IsTrue(ts2 <= ts1);
      Assert::IsTrue(ts1 <= ts3);
      Assert::IsTrue(ts2 <= ts3);
      Assert::IsTrue(!(ts3 <= ts1));
      Assert::IsTrue(!(ts3 <= ts2));

      // greater than or equal
      Assert::IsTrue(ts1 >= ts1);
      Assert::IsTrue(ts2 >= ts2);
      Assert::IsTrue(ts3 >= ts3);
      Assert::IsTrue(ts1 >= ts2);
      Assert::IsTrue(ts2 >= ts1);
      Assert::IsTrue(ts3 >= ts1);
      Assert::IsTrue(ts3 >= ts2);
      Assert::IsTrue(!(ts1 >= ts3));
      Assert::IsTrue(!(ts2 >= ts3));
   }

   TEST_METHOD(TestTimeZoneEnumerate)
   {
      std::vector<TimeZone> vecTimezones = TimeZone::EnumerateTimezones();
      Assert::IsTrue(vecTimezones.size() > 0);

      for (size_t i=0,iMax=vecTimezones.size(); i<iMax; i++)
      {
         TimeZone& timeZone = vecTimezones[i];

         ATLTRACE(_T("timezone %02u: %s / %s / %s"),
            i, timeZone.DisplayName(), timeZone.StandardName(), timeZone.DaylightName());
      }
   }

   TEST_METHOD(TestTimeZoneInDaylightSavingsTime1)
   {
      // note: test is dependent on timezone on computer
      TimeZone tz = TimeZone::System();

      // standard time -> daylight savings time
      Assert::IsTrue(false == tz.IsDaylightSavingTime(DateTime(2010, 2, 1, 0, 0, 0)));
      Assert::IsTrue(false == tz.IsDaylightSavingTime(DateTime(2010, 3, 1, 0, 0, 0)));
      Assert::IsTrue(true  == tz.IsDaylightSavingTime(DateTime(2010, 4, 1, 0, 0, 0)));
      Assert::IsTrue(true  == tz.IsDaylightSavingTime(DateTime(2010, 5, 1, 0, 0, 0)));

      Assert::IsTrue(false == tz.IsDaylightSavingTime(DateTime(2010, 3, 28, 0, 0, 0)));
      Assert::IsTrue(true  == tz.IsDaylightSavingTime(DateTime(2010, 3, 29, 0, 0, 0)));

      Assert::IsTrue(false == tz.IsDaylightSavingTime(DateTime(2010, 3, 28, 1, 0, 0)));
      Assert::IsTrue(false == tz.IsDaylightSavingTime(DateTime(2010, 3, 28, 2, 0, 0)));
      Assert::IsTrue(true  == tz.IsDaylightSavingTime(DateTime(2010, 3, 29, 3, 0, 0)));
      Assert::IsTrue(true  == tz.IsDaylightSavingTime(DateTime(2010, 3, 29, 4, 0, 0)));

      Assert::IsTrue(false == tz.IsDaylightSavingTime(DateTime(2010, 3, 28, 2, 0, 0)));
      Assert::IsTrue(true  == tz.IsDaylightSavingTime(DateTime(2010, 3, 29, 2, 0, 1)));
   }

   TEST_METHOD(TestTimeZoneInDaylightSavingsTime2)
   {
      // note: test is dependent on timezone on computer
      TimeZone tz = TimeZone::System();

      // daylight savings time -> standard time
      Assert::IsTrue(true  == tz.IsDaylightSavingTime(DateTime(2010, 9, 1, 0, 0, 0)));
      Assert::IsTrue(true  == tz.IsDaylightSavingTime(DateTime(2010, 10, 1, 0, 0, 0)));
      Assert::IsTrue(false == tz.IsDaylightSavingTime(DateTime(2010, 11, 1, 0, 0, 0)));
      Assert::IsTrue(false == tz.IsDaylightSavingTime(DateTime(2010, 12, 1, 0, 0, 0)));

      Assert::IsTrue(true  == tz.IsDaylightSavingTime(DateTime(2010, 10, 31, 0, 0, 0)));
      Assert::IsTrue(false == tz.IsDaylightSavingTime(DateTime(2010, 11, 1, 0, 0, 0)));

      Assert::IsTrue(true  == tz.IsDaylightSavingTime(DateTime(2010, 10, 31, 1, 0, 0)));
      Assert::IsTrue(true  == tz.IsDaylightSavingTime(DateTime(2010, 10, 31, 2, 0, 0)));
      Assert::IsTrue(false == tz.IsDaylightSavingTime(DateTime(2010, 10, 31, 3, 0, 0)));
      Assert::IsTrue(false == tz.IsDaylightSavingTime(DateTime(2010, 10, 31, 4, 0, 0)));

      Assert::IsTrue(true  == tz.IsDaylightSavingTime(DateTime(2010, 10, 31, 2, 59, 59)));
      Assert::IsTrue(false == tz.IsDaylightSavingTime(DateTime(2010, 10, 31, 3, 0, 0)));
   }
};

} // namespace UnitTest
