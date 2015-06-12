//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TraceOutputStopWatch.hpp Stopwatch class that does trace output
//
#pragma once

/// \brief Stopwatch that does trace output
/// \tparam T timer class to use; may be Timer or HighResolutionTimer
template <typename T>
class TraceOutputStopwatch
{
public:
   /// ctor; starts timer
   TraceOutputStopwatch(LPCTSTR pszName) throw()
      :m_pszName(pszName)
   {
      ATLTRACE(_T("Timer %s START\n"), pszName);
      m_timer.Start();
   }

   /// dtor; stops timer and does output
   ~TraceOutputStopwatch() throw()
   {
      double d = m_timer.TotalElapsed();
      ATLTRACE(_T("Timer %s STOP: "), m_pszName);
      if (d < 0.001)
         ATLTRACE(_T("0.%03u ms"), unsigned(d*1000.0*1000.0));
      else
      if (d < 1.0)
         ATLTRACE(_T("0.%03u s"), unsigned(d*1000.0));
      else
         ATLTRACE(_T("%u.%03u s"), unsigned(d), unsigned((d-unsigned(d))*1000.0));

      ATLTRACE(_T("\n"));
   }

private:
   LPCTSTR m_pszName;   ///< timer name
   T m_timer;           ///< timer class
};
