//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file HighResolutionTimer.cpp High resolution timer class
//

// includes
#include "stdafx.h"
#include "HighResolutionTimer.hpp"

/// returns performance counter frequency in counts / seconds
static LONGLONG GetPerformanceFrequency()
{
   static LARGE_INTEGER s_lFrequency = {0};
   if (s_lFrequency.QuadPart == 0LL)
   {
      ATLVERIFY(TRUE == ::QueryPerformanceFrequency(&s_lFrequency));
   }

   return s_lFrequency.QuadPart;
}

HighResolutionTimer::HighResolutionTimer() throw()
:m_bStarted(false)
{
   m_lCountStart.QuadPart = 0LL;
   m_lTotalElapsed.QuadPart = 0LL;
}

void HighResolutionTimer::Start() throw()
{
   ATLASSERT(m_bStarted == false);
   ATLVERIFY(TRUE == ::QueryPerformanceCounter(&m_lCountStart));
   m_bStarted = true;
}

void HighResolutionTimer::Stop() throw()
{
   ATLASSERT(m_bStarted == true);

   LARGE_INTEGER lCountNow = {0};
   ATLVERIFY(TRUE == ::QueryPerformanceCounter(&lCountNow));
   m_lTotalElapsed.QuadPart += (lCountNow.QuadPart-m_lCountStart.QuadPart);
   m_bStarted = false;
}

void HighResolutionTimer::Reset() throw()
{
   ATLASSERT(m_bStarted == false); // must be stopped when calling Reset()
   m_lTotalElapsed.QuadPart = 0;
}

void HighResolutionTimer::Restart() throw()
{
   // can either be stopped or started when restarting
   if (m_bStarted)
      Stop();
   Reset();
   Start();
}

double HighResolutionTimer::Elapsed() const throw()
{
   if (!m_bStarted)
      return 0.0; // no time elapsed, since timer isn't started

   LARGE_INTEGER lCountNow = {0};
   ATLVERIFY(TRUE == ::QueryPerformanceCounter(&lCountNow));
   return static_cast<double>(lCountNow.QuadPart-m_lCountStart.QuadPart) / GetPerformanceFrequency();
}

double HighResolutionTimer::TotalElapsed() const throw()
{
   LARGE_INTEGER lCountNow = {0};
   ATLVERIFY(TRUE == ::QueryPerformanceCounter(&lCountNow));
   return static_cast<double>(m_lTotalElapsed.QuadPart + (m_bStarted ? (lCountNow.QuadPart-m_lCountStart.QuadPart) : 0LL)) /
      GetPerformanceFrequency();
}
