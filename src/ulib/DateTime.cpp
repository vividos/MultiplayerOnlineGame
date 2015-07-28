//
// ulib - a collection of useful classes
// Copyright (C) 2006-2014 Michael Fink
//
/// \file DateTime.cpp date/time class
//

// includes
#include "stdafx.h"
#include <ulib/DateTime.hpp>
#include <ulib/Exception.hpp>
#include "ISO8601Parser.hpp"

// include Boost.DateTime
#define BOOST_DATE_TIME_NO_LIB

#pragma warning(push)
#pragma warning(disable: 4244) // 'argument' : conversion from 'T1' to 'T2', possible loss of data
#pragma warning(disable: 4245) // 'initializing' : conversion from 'T1' to 'T2', signed/unsigned mismatch
#include <boost/date_time.hpp>
#pragma warning(pop)

/// \brief time span implementation
/// \details uses boost::date_time classes
class TimeSpanImpl
{
public:
   /// ctor; initializes using special value
   TimeSpanImpl(boost::date_time::special_values sv = boost::date_time::not_a_date_time)
      :m_span(sv)
   {
   }

   /// ctor; initializes using given time_duration object
   TimeSpanImpl(const boost::posix_time::time_duration& span)
      :m_span(span)
   {
   }

   /// time duration object
   boost::posix_time::time_duration m_span;
};


TimeSpan::TimeSpan() throw()
:m_spImpl(new TimeSpanImpl)
{
}

TimeSpan::TimeSpan(int iHours, int iMins, int iSecs, int iMillisecs) throw()
:m_spImpl(new TimeSpanImpl)
{
   SetDateTimeSpan(iHours, iMins, iSecs, iMillisecs);
}

TimeSpan::TimeSpan(T_enStatus status)
:m_spImpl(new TimeSpanImpl)
{
   ATLASSERT(status == TimeSpan::min || status == TimeSpan::max);

   switch(status)
   {
   case TimeSpan::min: m_spImpl->m_span = boost::date_time::min_date_time; break;
   case TimeSpan::max: m_spImpl->m_span = boost::date_time::max_date_time; break;
   default:
      throw Exception(_T("invalid status in ctor"), __FILE__, __LINE__);
   }
}

TimeSpan::TimeSpan(const TimeSpan& dt)
:m_spImpl(dt.m_spImpl)
{
}

TimeSpan& TimeSpan::operator=(const TimeSpan& rhs)
{
   m_spImpl = rhs.m_spImpl;
   return *this;
}

TimeSpan::T_enStatus TimeSpan::Status() const throw()
{
   ATLASSERT(m_spImpl != NULL);

   if (m_spImpl->m_span.is_not_a_date_time())
      return TimeSpan::invalid;
   else if (m_spImpl->m_span == boost::date_time::min_date_time)
      return TimeSpan::min;
   else if (m_spImpl->m_span == boost::date_time::max_date_time)
      return TimeSpan::max;
   else
      return TimeSpan::valid;
}

int TimeSpan::Hours() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_span.hours();
}

int TimeSpan::Minutes() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_span.minutes();
}

int TimeSpan::Seconds() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_span.seconds();
}

int TimeSpan::Milliseconds() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return static_cast<int>(m_spImpl->m_span.fractional_seconds()) / 1000;
}

double TimeSpan::TotalHours() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return static_cast<double>(m_spImpl->m_span.ticks() / (3600*m_spImpl->m_span.ticks_per_second()));
}

double TimeSpan::TotalMinutes() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return static_cast<double>(m_spImpl->m_span.ticks() / (60*m_spImpl->m_span.ticks_per_second()));
}

double TimeSpan::TotalSeconds() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return static_cast<double>(m_spImpl->m_span.ticks() / (m_spImpl->m_span.ticks_per_second()));
}

double TimeSpan::TotalMilliseconds() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return static_cast<double>(m_spImpl->m_span.ticks() / (m_spImpl->m_span.ticks_per_second()/1000.0));
}

TimeSpan TimeSpan::operator+(const TimeSpan& rhs) const throw()
{
   ATLASSERT(m_spImpl != NULL);
   ATLASSERT(rhs.m_spImpl != NULL);

   TimeSpan dts;
   dts.m_spImpl->m_span = m_spImpl->m_span + rhs.m_spImpl->m_span;
   return dts;
}

TimeSpan TimeSpan::operator-(const TimeSpan& rhs) const throw()
{
   ATLASSERT(m_spImpl != NULL);
   ATLASSERT(rhs.m_spImpl != NULL);

   TimeSpan dts;
   dts.m_spImpl->m_span = m_spImpl->m_span - rhs.m_spImpl->m_span;
   return dts;
}

TimeSpan TimeSpan::operator*(int iFactor) const throw()
{
   ATLASSERT(m_spImpl != NULL);

   TimeSpan dts;
   dts.m_spImpl->m_span = m_spImpl->m_span * iFactor;
   return dts;
}

TimeSpan TimeSpan::operator/(int iFactor) const throw()
{
   ATLASSERT(m_spImpl != NULL);

   TimeSpan dts;
   dts.m_spImpl->m_span = m_spImpl->m_span / iFactor;
   return dts;
}

TimeSpan& TimeSpan::operator+=(const TimeSpan span) throw()
{
   ATLASSERT(m_spImpl != NULL);
   ATLASSERT(span.m_spImpl != NULL);

   PrepareCopy();

   m_spImpl->m_span += span.m_spImpl->m_span;
   return *this;
}

TimeSpan& TimeSpan::operator-=(const TimeSpan span) throw()
{
   ATLASSERT(m_spImpl != NULL);
   ATLASSERT(span.m_spImpl != NULL);

   PrepareCopy();

   m_spImpl->m_span -= span.m_spImpl->m_span;
   return *this;
}

TimeSpan& TimeSpan::operator*=(int iFactor) throw()
{
   ATLASSERT(m_spImpl != NULL);

   PrepareCopy();

   m_spImpl->m_span *= iFactor;
   return *this;
}

TimeSpan& TimeSpan::operator/=(int iFactor) throw()
{
   ATLASSERT(m_spImpl != NULL);

   PrepareCopy();

   m_spImpl->m_span /= iFactor;
   return *this;
}

TimeSpan TimeSpan::operator-() const throw()
{
   ATLASSERT(m_spImpl != NULL);

   TimeSpan dts(*this);
   dts.PrepareCopy();
   dts.m_spImpl->m_span = -dts.m_spImpl->m_span;

   return dts;
}

bool TimeSpan::operator==(const TimeSpan& rhs) const throw()
{
   ATLASSERT(m_spImpl != NULL);
   ATLASSERT(rhs.m_spImpl != NULL);

   return m_spImpl->m_span == rhs.m_spImpl->m_span;
}

bool TimeSpan::operator!=(const TimeSpan& rhs) const throw()
{
   ATLASSERT(m_spImpl != NULL);
   ATLASSERT(rhs.m_spImpl != NULL);

   return m_spImpl->m_span != rhs.m_spImpl->m_span;
}

bool TimeSpan::operator>(const TimeSpan& rhs) const throw()
{
   ATLASSERT(m_spImpl != NULL);
   ATLASSERT(rhs.m_spImpl != NULL);

   return m_spImpl->m_span > rhs.m_spImpl->m_span;
}

bool TimeSpan::operator<(const TimeSpan& rhs) const throw()
{
   ATLASSERT(m_spImpl != NULL);
   ATLASSERT(rhs.m_spImpl != NULL);

   return m_spImpl->m_span < rhs.m_spImpl->m_span;
}

void TimeSpan::SetDateTimeSpan(int iHours, int iMins, int iSecs, int iMillisecs) throw()
{
   ATLASSERT(m_spImpl != NULL);

   PrepareCopy();

   try
   {
      // note: multiplying milliseconds with 1000 here, since resolution is in microsecs
      m_spImpl->m_span = boost::posix_time::time_duration(iHours, iMins, iSecs, iMillisecs*1000);
   }
   catch(...)
   {
      // illegal date/time span value
      m_spImpl->m_span = boost::date_time::not_a_date_time;
   }
}

CString TimeSpan::FormatISO8601() const throw()
{
   ATLASSERT(m_spImpl != NULL);

   if (m_spImpl->m_span.is_not_a_date_time())
      return _T("");

   CString cszFormat(_T("P%H:%M:%S"));

   struct tm tmTemp = boost::posix_time::to_tm(m_spImpl->m_span);

   CString cszDateTime;
   LPTSTR pszBuffer = cszDateTime.GetBufferSetLength(256);
   _tcsftime(pszBuffer, cszDateTime.GetLength(), cszFormat, &tmTemp);
   cszDateTime.ReleaseBuffer();

   return cszDateTime;
}

void TimeSpan::PrepareCopy() throw()
{
   ATLASSERT(m_spImpl != NULL);

   if (!m_spImpl.unique())
      m_spImpl.reset(new TimeSpanImpl(m_spImpl->m_span));
}

// ///////////////////////////////////

/// \brief DateTime implementation
/// \details uses boost::date_time classes
class DateTimeImpl
{
public:
   /// ctor; initializes using special value
   DateTimeImpl(boost::date_time::special_values sv = boost::date_time::not_a_date_time)
      :m_dt(sv)
   {
   }

   /// ctor; initializes using existing ptime object
   DateTimeImpl(const boost::posix_time::ptime& dt)
      :m_dt(dt)
   {
   }

   /// posix time object
   boost::posix_time::ptime m_dt;
};

DateTime::DateTime()
:m_spImpl(new DateTimeImpl)
{
}

DateTime::DateTime(unsigned int uiYear, unsigned int uiMonth, unsigned int uiDay,
   unsigned int uiHour, unsigned int uiMinute, unsigned int uiSecond, unsigned int uiMillisecond)
:m_spImpl(new DateTimeImpl)
{
   try
   {
      m_spImpl->m_dt = boost::posix_time::ptime(
         boost::gregorian::date(USHORT(uiYear), USHORT(uiMonth), USHORT(uiDay)),
         boost::posix_time::time_duration(uiHour, uiMinute, uiSecond, uiMillisecond*1000));
   }
   catch(...)
   {
      // illegal date/time value(s)
      m_spImpl->m_dt = boost::date_time::not_a_date_time;
   }
}

DateTime::DateTime(T_enStatus status)
:m_spImpl(new DateTimeImpl)
{
   ATLASSERT(status == DateTime::min || status == DateTime::max);

   switch(status)
   {
   case DateTime::min: m_spImpl->m_dt = boost::date_time::min_date_time; break;
   case DateTime::max: m_spImpl->m_dt = boost::date_time::max_date_time; break;
   default:
      throw Exception(_T("invalid status in ctor"), __FILE__, __LINE__);
   }
}

DateTime::DateTime(const DateTime& dt)
:m_spImpl(dt.m_spImpl) // copy on write
{
}

DateTime& DateTime::operator=(const DateTime& rhs)
{
   m_spImpl = rhs.m_spImpl; // copy on write
   return *this;
}

DateTime DateTime::Now() throw()
{
/*#ifndef _WIN32_WCE
   time_t now = time(NULL);
   struct tm tmNow = *gmtime(&now);

   return DateTime(
      tmNow.tm_year+1900,  tmNow.tm_mon+1,   tmNow.tm_mday,
      tmNow.tm_hour,       tmNow.tm_min,     tmNow.tm_sec);
#else
*/
   SYSTEMTIME stNow;
   GetSystemTime(&stNow);
   return DateTime(stNow.wYear, stNow.wMonth, stNow.wDay,
      stNow.wHour, stNow.wMinute, stNow.wSecond, stNow.wMilliseconds);
//#endif
}

DateTime DateTime::Today() throw()
{
   DateTime dt = DateTime::Now();

   dt.m_spImpl->m_dt = boost::posix_time::ptime(dt.m_spImpl->m_dt.date());

   return dt;
}


void DateTime::SetDateTime(unsigned int uiYear, unsigned int uiMonth, unsigned int uiDay,
   unsigned int uiHour, unsigned int uiMinute, unsigned int uiSecond, unsigned int uiMillisecond) throw()
{
   PrepareCopy();

   try
   {
      m_spImpl->m_dt = boost::posix_time::ptime(
         boost::gregorian::date(USHORT(uiYear), USHORT(uiMonth), USHORT(uiDay)),
         boost::posix_time::time_duration(uiHour, uiMinute, uiSecond, uiMillisecond*1000));
   }
   catch(...)
   {
      // illegal date/time span value
      m_spImpl->m_dt = boost::date_time::not_a_date_time;
   }
}

DateTime::T_enStatus DateTime::Status() const throw()
{
   ATLASSERT(m_spImpl != NULL);

   if (m_spImpl->m_dt.is_not_a_date_time())
      return DateTime::invalid;
   else if (m_spImpl->m_dt == boost::date_time::min_date_time)
      return DateTime::min;
   else if (m_spImpl->m_dt == boost::date_time::max_date_time)
      return DateTime::max;
   else
      return DateTime::valid;
}

unsigned int DateTime::Year() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt.date().year();
}

unsigned int DateTime::Month() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt.date().month();
}

unsigned int DateTime::Day() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt.date().day();
}

unsigned int DateTime::Hour() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt.time_of_day().hours();
}

unsigned int DateTime::Minute() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt.time_of_day().minutes();
}

unsigned int DateTime::Second() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt.time_of_day().seconds();
}

unsigned int DateTime::Millisecond() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return static_cast<unsigned int>(m_spImpl->m_dt.time_of_day().fractional_seconds()) / 1000;
}

unsigned int DateTime::DayOfWeek() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   // day of week returns numbers from 0..6, 0 being sunday
   return m_spImpl->m_dt.date().day_of_week().as_number();
}

unsigned int DateTime::DayOfYear() const throw()
{
   ATLASSERT(m_spImpl != NULL);
   // day of year returns numbers from 1..366
   return m_spImpl->m_dt.date().day_of_year();
}

TimeSpan DateTime::TimeOfDay() throw()
{
   return TimeSpan(Hour(), Minute(), Second(), Millisecond());
}

// operators

DateTime DateTime::operator+(const TimeSpan& rhs) const throw()
{
   DateTime dt(*this);
   dt += rhs;
   return dt;
}

TimeSpan DateTime::operator-(const DateTime& rhs) const throw()
{
   ATLASSERT(m_spImpl != NULL);

   TimeSpan span;
   span.m_spImpl->m_span = m_spImpl->m_dt - rhs.m_spImpl->m_dt;
   return span;
}

DateTime& DateTime::operator+=(const TimeSpan& span) throw()
{
   ATLASSERT(m_spImpl != NULL);
   PrepareCopy();
   m_spImpl->m_dt += span.m_spImpl->m_span;
   return *this;
}

DateTime& DateTime::operator-=(const TimeSpan& span) throw()
{
   ATLASSERT(m_spImpl != NULL);
   PrepareCopy();
   m_spImpl->m_dt -= span.m_spImpl->m_span;
   return *this;
}

DateTime DateTime::operator-(const TimeSpan& rhs) const throw()
{
   DateTime dt(*this);
   dt -= rhs;
   return dt;
}

bool DateTime::operator==(const DateTime& rhs) const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt == rhs.m_spImpl->m_dt;
}

bool DateTime::operator!=(const DateTime& rhs) const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt != rhs.m_spImpl->m_dt;
}

bool DateTime::operator>(const DateTime& rhs) const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt > rhs.m_spImpl->m_dt;
}

bool DateTime::operator<(const DateTime& rhs) const throw()
{
   ATLASSERT(m_spImpl != NULL);
   return m_spImpl->m_dt < rhs.m_spImpl->m_dt;
}

CString DateTime::FormatISO8601(T_enISO8601Format enFormat, bool bBasic, const TimeZone& tz) const throw()
{
   CString cszDate;
   switch(enFormat)
   {
   case formatY:  return Format(_T("%Y"), tz);
   case formatYM: return Format(bBasic ? _T("%Y%m") : _T("%Y-%m"), tz);
   case formatYMD: return Format(bBasic ? _T("%Y%m%d") : _T("%Y-%m-%d"), tz);
   case formatYMD_HM_Z:
      cszDate = Format(bBasic ? _T("%Y%m%dT%H%M") : _T("%Y-%m-%dT%H:%M"), tz);
      break;

   case formatYMD_HMS_Z:
      cszDate = Format(bBasic ? _T("%Y%m%dT%H%M%S") : _T("%Y-%m-%dT%H:%M:%S"), tz);
      break;

   case formatYMD_HMSF_Z:
      {
         cszDate = Format(bBasic ? _T("%Y%m%dT%H%M%S") : _T("%Y-%m-%dT%H:%M:%S"), tz);

         CString cszFraction;
         cszFraction.Format(_T(".%03u"), Millisecond());
         cszDate += cszFraction;
      }
      break;
   }

   // add timezone
   if (tz.StandardName() == _T("UTC"))
      cszDate += _T("Z");
   else
   {
      TimeSpan spTimezone = tz.GetUtcOffset(*this);
      bool bNegative = spTimezone < TimeSpan(0, 0, 0, 0);

      TimeSpan spTimezoneAbs = bNegative ? -spTimezone : spTimezone;

      CString cszTimezone;
      cszTimezone.Format(bBasic ? _T("%c%02u%02u") : _T("%c%02u:%02u"),
         !bNegative ? _T('+') : _T('-'),
         spTimezoneAbs.Hours(),
         spTimezoneAbs.Minutes());

      cszDate += cszTimezone;
   }

   return cszDate;
}

CString DateTime::Format(const CString& cszFormat, const TimeZone& tz) const throw()
{
   ATLASSERT(m_spImpl != NULL);

   if (m_spImpl->m_dt.is_not_a_date_time())
      return _T("");

   // calculate offset to UTC
   TimeSpan tsTimezone = tz.GetUtcOffset(*this);

   struct tm tmTemp = boost::posix_time::to_tm(m_spImpl->m_dt + tsTimezone.m_spImpl->m_span);

   CString cszDateTime;
   LPTSTR pszBuffer = cszDateTime.GetBufferSetLength(256);
   _tcsftime(pszBuffer, cszDateTime.GetLength(), cszFormat, &tmTemp);
   cszDateTime.ReleaseBuffer();

   return cszDateTime;
}

void DateTime::ParseISO8601(const CString& cszISO8601Timestamp) const throw()
{
   if (cszISO8601Timestamp.IsEmpty())
   {
      m_spImpl->m_dt = boost::date_time::not_a_date_time;
      return;
   }

   ISO8601Parser p(cszISO8601Timestamp);
   if (p.IsValid())
      m_spImpl->m_dt = p.Get();
}

void DateTime::PrepareCopy() throw()
{
   ATLASSERT(m_spImpl != NULL);

   if (!m_spImpl.unique())
      m_spImpl.reset(new DateTimeImpl(m_spImpl->m_dt));
}
