//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file HighResolutionTimer.hpp High resolution timer class
//
#pragma once

// includes
#include "Base.hpp"

/// \brief High resolution timer class
/// \details Has a high precision. Has identical interface as class Timer.
/// Can be used in TraceOutputStopwatch.
class BASE_DECLSPEC HighResolutionTimer
{
public:
   /// ctor
   HighResolutionTimer() throw();

   /// starts timer; timer must be stopped or reset
   void Start() throw();

   /// stops timer; timer must be started
   void Stop() throw();

   /// resets timer; timer must be stopped
   void Reset() throw();

   /// restarts timer by resetting and starting again
   void Restart() throw();

   /// returns elapsed time since Start() was called, in seconds
   double Elapsed() const throw();

   /// returns total elapsed time in seconds
   double TotalElapsed() const throw();

   /// returns if timer is running
   bool IsStarted() const throw() { return m_bStarted; }

private:
   /// indicates if timer is started
   bool m_bStarted;

   /// start time
   LARGE_INTEGER m_lCountStart;

   /// already elapsed time
   LARGE_INTEGER m_lTotalElapsed;
};
