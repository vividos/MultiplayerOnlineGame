//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file WorldRunner.cpp World runner
//

// includes
#include "StdAfx.h"
#include "WorldRunner.hpp"
#include "WorldModel.hpp"

/// world tick cycle, in milliseconds
const unsigned int c_uiWorldTickCycleInMilliseconds = 100;

void WorldRunner::Start()
{
   m_timerWorldTick.expires_from_now(boost::posix_time::milliseconds(c_uiWorldTickCycleInMilliseconds));

   m_timerWorldTick.async_wait(
      std::bind(&WorldRunner::OnTimerWorldTick, this, std::placeholders::_1));

   m_ioServiceThread.Run();
}

void WorldRunner::Stop()
{
   m_timerWorldTick.cancel();
   m_evtWaitEndTimer.Wait();
   m_ioServiceThread.Join();
}

void WorldRunner::OnTimerWorldTick(const boost::system::error_code& error)
{
   if (error)
   {
      m_evtWaitEndTimer.Set();
      return;
   }

   // world tick occurs in fixed intervals; so set next point, then process
   m_timerWorldTick.expires_from_now(boost::posix_time::milliseconds(c_uiWorldTickCycleInMilliseconds));

   ProcessWorldTick();

   m_timerWorldTick.async_wait(
      boost::bind(&WorldRunner::OnTimerWorldTick, this, _1));
}

void WorldRunner::ProcessWorldTick()
{
   HighResolutionTimer timerTick;
   timerTick.Start();

   m_worldModel.Tick(m_timeBase.Now());

   timerTick.Stop();

   // check elapsed time if processing took >= 80%
   if (timerTick.Elapsed() >= c_uiWorldTickCycleInMilliseconds / 1000.0 * 0.8)
   {
      CString cszText;
      cszText.Format(_T("WorldModel::Tick() used %u ms of %u ms"),
         static_cast<unsigned int>(timerTick.Elapsed() * 1000),
         c_uiWorldTickCycleInMilliseconds);

      LOG_WARN(cszText, Log::Server::General)
   }
}
