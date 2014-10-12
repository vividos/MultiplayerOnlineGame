//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file WorldRunner.hpp World runner
//
#pragma once

// includes
#include "ServerLogic.hpp"
#include "IoServiceThread.hpp"
#include <ulib/Event.hpp>
#include "TimeBase.hpp"

// forward references
class WorldModel;

/// world runner
class SERVERLOGIC_DECLSPEC WorldRunner
{
public:
   /// ctor
   WorldRunner(WorldModel& worldModel)
      :m_worldModel(worldModel),
       m_ioServiceThread(true, _T("World Runner Thread")),
       m_timerWorldTick(m_ioServiceThread.Get()),
       m_evtWaitEndTimer(true, false) // manual-reset event
   {
   }

   /// starts world processing; returns immediately
   void Start();

   /// stops world processing
   void Stop();

private:
   /// timer callback to do world tick
   void OnTimerWorldTick(const boost::system::error_code& error);

   /// process world tick
   void ProcessWorldTick();

private:
   /// world model to run
   WorldModel& m_worldModel;

   /// game time base
   TimeBase m_timeBase;

   /// service thread
   IoServiceThread m_ioServiceThread;

   /// timer for world tick
   boost::asio::deadline_timer m_timerWorldTick;

   /// event to signal stop processing
   Event m_evtWaitEndTimer;
};
