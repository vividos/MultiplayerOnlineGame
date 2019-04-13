//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TimeBase.hpp Game time base
//
#pragma once

// includes
#include "Common.hpp"
#include <ulib/HighResolutionTimer.hpp>

/// game time index
class TimeIndex
{
public:
   /// ctor
   TimeIndex(double dTime = 0.0)
      :m_dTime(dTime)
   {
   }

   /// returns current time index
   double Get() const { return m_dTime; }

   /// less operator
   bool operator<(const TimeIndex& rhs) const { return m_dTime < rhs.m_dTime; }

private:
   /// time index value
   double m_dTime;
};

/// time base used in the game
class COMMON_DECLSPEC TimeBase
{
public:
   /// ctor; initializes time base
   TimeBase();

   /// returns seconds and fractions since epoch;
   TimeIndex Now() const;

private:
   /// offset to epoch in seconds
   double m_dEpochOffset;

   /// a timer that measures time since start
   HighResolutionTimer m_timer;
};
