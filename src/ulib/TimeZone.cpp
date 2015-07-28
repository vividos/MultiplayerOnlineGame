//
// ulib - a collection of useful classes
// Copyright (C) 2006-2015 Michael Fink
//
/// \file TimeZone.cpp time zone class
//

// includes
#include "stdafx.h"
#include <ulib/TimeZone.hpp>
#include <ulib/DateTime.hpp>
#include <ulib/Exception.hpp>
#include <algorithm>

/// registry path with time zone infos
LPCTSTR g_pszRegTimeZonesPath = _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones");

/// timezone implementation
class TimeZoneImpl
{
public:
   /// ctor
   TimeZoneImpl() throw()
   {
      ZeroMemory(&m_tzi, sizeof(m_tzi));
   }

   /// display name
   CString m_cszDisplayName;

   /// timezone info
   TIME_ZONE_INFORMATION m_tzi;
};

TimeZone::TimeZone()
:m_spImpl(new TimeZoneImpl)
{
}

TimeZone TimeZone::UTC() throw()
{
   TimeZone tz;
   _tcsncpy_s(tz.m_spImpl->m_tzi.StandardName, sizeof(tz.m_spImpl->m_tzi.StandardName) / sizeof(TCHAR),
      _T("UTC"), sizeof(tz.m_spImpl->m_tzi.StandardName) / sizeof(TCHAR));
   return tz;
}

TimeZone TimeZone::System()
{
   TimeZone tz;
   DWORD dwRet = GetTimeZoneInformation(&tz.m_spImpl->m_tzi);

   if (TIME_ZONE_ID_INVALID == dwRet)
      throw Exception(_T("couldn't get system timezone info"), __FILE__, __LINE__);

   return tz;
}

#pragma pack(push)
#pragma pack(1)

/// registry timezone info format
struct REG_TZI_FORMAT
{
   long Bias;              ///< bias in minutes
   long StandardBias;      ///< bias in minutes, during standard time
   long DaylightBias;      ///< bias in minutes, during daylight savings time
   SYSTEMTIME StandardDate;///< date and local time for transistion from daylight savings time to standard time
   SYSTEMTIME DaylightDate;///< date and local time for transistion from standard time to daylight savings time
};

#pragma pack(pop)

std::vector<TimeZone> TimeZone::EnumerateTimezones()
{
   std::vector<TimeZone> vecTimezones;

   CRegKey regKeyTimeZones;
   if (ERROR_SUCCESS != regKeyTimeZones.Open(HKEY_LOCAL_MACHINE, g_pszRegTimeZonesPath,
      KEY_ENUMERATE_SUB_KEYS))
      throw Exception(_T("couldn't enumerate timezones"), __FILE__, __LINE__);

   DWORD dwSize = 256;
   TCHAR szBuffer[256];
   DWORD dwIndex = 0;
   while (ERROR_SUCCESS == ::RegEnumKeyEx(regKeyTimeZones.m_hKey, dwIndex++, szBuffer, &dwSize, NULL, NULL, NULL, NULL))
   {
      CRegKey regKeyTimeZoneInfo;
      if (ERROR_SUCCESS != regKeyTimeZoneInfo.Open(regKeyTimeZones,
         szBuffer, KEY_QUERY_VALUE))
         throw Exception(_T("couldn't enumerate timezones"), __FILE__, __LINE__);

      TimeZone timeZone;

      // read time zone names
      ULONG ulCount = sizeof(timeZone.m_spImpl->m_tzi.StandardName) / sizeof(TCHAR) - 1;
      regKeyTimeZoneInfo.QueryStringValue(_T("Std"), timeZone.m_spImpl->m_tzi.StandardName, &ulCount);

      ulCount = sizeof(timeZone.m_spImpl->m_tzi.DaylightName) / sizeof(TCHAR) - 1;
      regKeyTimeZoneInfo.QueryStringValue(_T("Dlt"), timeZone.m_spImpl->m_tzi.DaylightName, &ulCount);

      ulCount = 256;
      regKeyTimeZoneInfo.QueryStringValue(_T("Display"), timeZone.m_spImpl->m_cszDisplayName.GetBuffer(ulCount), &ulCount);
      timeZone.m_spImpl->m_cszDisplayName.ReleaseBuffer(ulCount / sizeof(TCHAR));

      // get type and length
      DWORD dwType = 0, dwLength = 0;
      if (ERROR_SUCCESS != RegQueryValueEx(regKeyTimeZoneInfo.m_hKey, _T("TZI"), NULL, &dwType, NULL, &dwLength) ||
         dwLength != 44 || dwType != REG_BINARY)
         throw Exception(_T("couldn't enumerate timezones"), __FILE__, __LINE__);

      REG_TZI_FORMAT tzi;

      // get binary data
      static_assert(sizeof(REG_TZI_FORMAT) == 44, "struct REG_TZI_FORMAT must have a size of 44");

      if (ERROR_SUCCESS != RegQueryValueEx(regKeyTimeZoneInfo.m_hKey, _T("TZI"), NULL, NULL, reinterpret_cast<LPBYTE>(&tzi), &dwLength))
         throw Exception(_T("couldn't enumerate timezones"), __FILE__, __LINE__);

      timeZone.m_spImpl->m_tzi.Bias = tzi.Bias;
      timeZone.m_spImpl->m_tzi.StandardBias = tzi.StandardBias;
      timeZone.m_spImpl->m_tzi.DaylightBias = tzi.DaylightBias;
      timeZone.m_spImpl->m_tzi.StandardDate = tzi.StandardDate;
      timeZone.m_spImpl->m_tzi.DaylightDate = tzi.DaylightDate;

      vecTimezones.push_back(timeZone);

      dwSize = 256; // prepare for next call to RegEnumKeyEx
   }

   std::sort(vecTimezones.begin(), vecTimezones.end(), CompareGreaterBias);

   return vecTimezones;
}

CString TimeZone::DaylightName() const throw()
{
   ATLASSERT(m_spImpl != nullptr);

   return m_spImpl->m_tzi.DaylightName;
}

CString TimeZone::StandardName() const throw()
{
   ATLASSERT(m_spImpl != nullptr);

   return m_spImpl->m_tzi.StandardName;
}

CString TimeZone::DisplayName() const throw()
{
   ATLASSERT(m_spImpl != nullptr);

   return m_spImpl->m_cszDisplayName;
}

TimeSpan TimeZone::GetUtcOffset(const DateTime& dt) const
{
   ATLASSERT(m_spImpl != nullptr);

   if (!IsDaylightSavingTime(dt))
      return TimeSpan(0, -(m_spImpl->m_tzi.Bias + m_spImpl->m_tzi.StandardBias), 0, 0);
   else
      return TimeSpan(0, -(m_spImpl->m_tzi.Bias + m_spImpl->m_tzi.DaylightBias), 0, 0);
}

/// converts SYSTEMTIME to DateTime
DateTime SystemTimeToDateTime(const SYSTEMTIME& st)
{
   return DateTime(
      st.wYear,
      st.wMonth,
      st.wDay,
      st.wHour,
      st.wMinute,
      st.wSecond,
      st.wMilliseconds);
}

/// from:
/// http://www.ptgsystems.com/blognet/post/2010/03/12/determining-the-nth-occurence-of-a-day-of-the-week-in-any-given-month.aspx
DateTime TimeZone::CalculateTransitionDate(unsigned int uiYear, unsigned int uiIndex) const
{
   ATLASSERT(m_spImpl != nullptr);
   ATLASSERT(uiIndex == 0 || uiIndex == 1);

   const SYSTEMTIME& st = uiIndex == 0 ? m_spImpl->m_tzi.DaylightDate : m_spImpl->m_tzi.StandardDate;
   SYSTEMTIME stTemp = st;

   if (stTemp.wYear != 0 ||                           // one-time switch
      ((stTemp.wDay  < 1) || (stTemp.wDay > 5)) ||  // invalid number of week
      stTemp.wDayOfWeek > 6)                         // invalid day of week
   {
      ATLASSERT(false);
      return DateTime(DateTime::invalid);
   }

   stTemp.wYear = static_cast<WORD>(uiYear); // set year

   stTemp.wDay = 1;
   FILETIME ft;
   SystemTimeToFileTime(&stTemp, &ft);
   FileTimeToSystemTime(&ft, &stTemp);

   int nDayOff;

   // determine offset to first target WeekDay
   nDayOff = (int)st.wDayOfWeek - (int)stTemp.wDayOfWeek;
   if (nDayOff < 0)
      nDayOff += 7;

   // offset to wDay occurence of WeekDay
   nDayOff += ((st.wDay - 1) * 7);
   stTemp.wDay = static_cast<WORD>(nDayOff + 1);
   BOOL bValid = SystemTimeToFileTime(&stTemp, &ft);
   if (bValid)
   {
      SYSTEMTIME stFinal = { 0 };
      FileTimeToSystemTime(&ft, &stFinal);

      return SystemTimeToDateTime(stFinal);
   }
   else if (st.wDay == 5)    // Only allow backup of 1 week if wDay = 5 (last occurrence in month)
   {
      nDayOff -= 7;
      stTemp.wDay = static_cast<WORD>(nDayOff + 1);
      bValid = SystemTimeToFileTime(&stTemp, &ft);

      SYSTEMTIME stFinal = { 0 };
      FileTimeToSystemTime(&ft, &stFinal);

      if (bValid)
         return SystemTimeToDateTime(stFinal);
   }

   return DateTime(DateTime::invalid);
}

bool TimeZone::IsDaylightSavingTime(const DateTime& dt) const
{
   ATLASSERT(m_spImpl != nullptr);

   // check if there are valid transition dates at all
   if (m_spImpl->m_tzi.StandardDate.wMonth == 0 ||
      m_spImpl->m_tzi.DaylightDate.wMonth == 0)
      return false; // no daylight savings time without transition dates

                    // convert transition dates to absolute dates
   DateTime dtTrans1 = CalculateTransitionDate(dt.Year(), 0);
   DateTime dtTrans2 = CalculateTransitionDate(dt.Year(), 1);

   if (dtTrans1.Status() != DateTime::valid &&
      dtTrans2.Status() != DateTime::valid)
      return false; // no daylight savings time

                    //ATLTRACE(_T("t1=%s\nt2=%s\ndt=%s"),
                    //   dtTrans1.Format(_T("%Y-%m-%dT%H:%M:%S")),
                    //   dtTrans2.Format(_T("%Y-%m-%dT%H:%M:%S")),
                    //   dt.Format(_T("%Y-%m-%dT%H:%M:%S")));

                    // compare if given date/time is in the range
   return dtTrans1 < dt && dt < dtTrans2;
}

bool TimeZone::CompareLessBias(const TimeZone& lhs, const TimeZone& rhs)
{
   return lhs.m_spImpl->m_tzi.Bias < rhs.m_spImpl->m_tzi.Bias;
}

bool TimeZone::CompareGreaterBias(const TimeZone& lhs, const TimeZone& rhs)
{
   return lhs.m_spImpl->m_tzi.Bias > rhs.m_spImpl->m_tzi.Bias;
}
