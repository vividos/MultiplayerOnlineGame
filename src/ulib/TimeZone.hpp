//
// ulib - a collection of useful classes
// Copyright (C) 2006-2015 Michael Fink
//
/// \file TimeZone.hpp time zone class
//
#pragma once

// includes
#include <ulib/config/AutoLink.hpp>
#include <vector>
#include <memory>

// forward references
class DateTime;
class TimeSpan;

/// represents a single timezone
class ULIB_DECLSPEC TimeZone
{
public:
   /// returns UTC timezone
   static TimeZone UTC() throw();

   /// returns current system's timezone
   static TimeZone System();

   /// enumerates all timezones in the system
   static std::vector<TimeZone> EnumerateTimezones();

   /// returns timezone name during daylight savings time
   CString DaylightName() const throw();

   /// returns timezone name during standard time
   CString StandardName() const throw();

   /// returns display name for timezone when enumerating all system timezones
   CString DisplayName() const throw();

   /// gets UTC offset for given date/time
   TimeSpan GetUtcOffset(const DateTime& dt) const;

   /// returns if date/time is in daylight savings time
   bool IsDaylightSavingTime(const DateTime& dt) const;

   /// compare function for std::sort; sorts by bias
   static bool CompareLessBias(const TimeZone& lhs, const TimeZone& rhs);

   /// compare function for std::sort; sorts by bias
   static bool CompareGreaterBias(const TimeZone& lhs, const TimeZone& rhs);

private:
   TimeZone();

   /// calculates transition time for daylight savings time (index=0) or standard time (index=1)
   DateTime CalculateTransitionDate(unsigned int uiYear, unsigned int uiIndex) const;

private:
   /// implementation
   std::shared_ptr<class TimeZoneImpl> m_spImpl;
};
