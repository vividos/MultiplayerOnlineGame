//
// ulib - a collection of useful classes
// Copyright (C) 2006-2015 Michael Fink
//
/// \file StdioTimeZone.cpp time zone class using stdio headers
//

// includes
#include "stdafx.h"
#include <ulib/TimeZone.hpp>
#include <ulib/DateTime.hpp>

// include Boost.DateTime
#define BOOST_DATE_TIME_NO_LIB
#include <boost/date_time/time_zone_base.hpp>

/// timezone implementation
class TimeZoneImpl
{
public:
   /// ctor
   TimeZoneImpl() throw()
   {
   }

   // TODO
};

TimeZone::TimeZone()
   :m_spImpl(new TimeZoneImpl)
{
}

TimeZone TimeZone::UTC() throw()
{
   TimeZone tz;

   // TODO

   return tz;
}

TimeZone TimeZone::System()
{
   TimeZone tz;

   // TODO

   return tz;
}

std::vector<TimeZone> TimeZone::EnumerateTimezones()
{
   std::vector<TimeZone> vecTimezones;

   // TODO

   return vecTimezones;
}

CString TimeZone::DaylightName() const throw()
{
   ATLASSERT(m_spImpl != nullptr);

   // TODO

   return _T("");
}

CString TimeZone::StandardName() const throw()
{
   ATLASSERT(m_spImpl != nullptr);

   // TODO

   return _T("");
}

CString TimeZone::DisplayName() const throw()
{
   ATLASSERT(m_spImpl != nullptr);

   // TODO

   return _T("");
}

TimeSpan TimeZone::GetUtcOffset(const DateTime& dt) const
{
   ATLASSERT(m_spImpl != nullptr);

   // TODO

   return TimeSpan();
}

DateTime TimeZone::CalculateTransitionDate(unsigned int uiYear, unsigned int uiIndex) const
{
   ATLASSERT(m_spImpl != nullptr);
   ATLASSERT(uiIndex == 0 || uiIndex == 1);

   // TODO

   return DateTime(DateTime::invalid);
}

bool TimeZone::IsDaylightSavingTime(const DateTime& dt) const
{
   ATLASSERT(m_spImpl != nullptr);

   // TODO
   return false;
}
