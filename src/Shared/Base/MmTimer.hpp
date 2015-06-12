//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file MmTimer.hpp Timer class
//
#pragma once

/// \brief Multimedia timer class
/// \details Has a precision of about 1 ms. Has identical interface as class
/// HighResolutionTimer. Can be used in TraceOutputStopwatch.
class MmTimer
{
public:
   /// ctor
   MmTimer() throw()
      :m_bStarted(false),
       m_dwStart(0),
       m_dwElapsed(0)
   {
   }

   /// starts timer; timer must be stopped or reset
   void Start() throw()
   {
      ATLASSERT(m_bStarted == false);
      m_dwStart = timeGetTime();
      m_bStarted = true;
   }

   /// stops timer; timer must be started
   void Stop() throw()
   {
      ATLASSERT(m_bStarted == true);

      DWORD dwNow = timeGetTime();
      m_dwTotalElapsed += (dwNow-m_dwStart);
      m_bStarted = false;
   }

   /// resets timer; timer must be stopped
   void Reset() throw()
   {
      ATLASSERT(m_bStarted == false); // must be stopped when calling Reset()
      m_dwTotalElapsed = 0;
   }

   /// restarts timer by resetting and starting again
   void Restart() throw()
   {
      // can either be stopped or started when restarting
      if (m_bStarted)
         Stop();
      Reset();
      Start();
   }

   /// returns elapsed time since Start() was called, in seconds
   double Elapsed() const throw()
   {
      if (!m_bStarted)
         return 0.0; // no time elapsed, since timer isn't started

      DWORD dwNow = timeGetTime();
      return 0.001 * (dwNow-m_dwStart);
   }

   /// returns total elapsed time in seconds
   double TotalElapsed() const throw()
   {
      DWORD dwNow = timeGetTime();
      return 0.001 * (m_dwTotalElapsed + (m_bStarted ? (dwNow-m_dwStart) : 0));
   }

   /// returns if timer is running
   bool IsStarted() const throw() { return m_bStarted; }

private:
   /// indicates if timer is started
   bool m_bStarted;

   /// start time
   DWORD m_dwStart;

   /// time already elapsed
   DWORD m_dwTotalElapsed;
};
