//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file TaskManager.hpp Background task manager
//
#pragma once

// include
#include "TaskGroup.hpp"
#include "IoServiceThread.hpp"

/// \brief background task manager
class TaskManager
{
public:
   DEFINE_INSTANCE(TaskManager)

   /// ctor
   TaskManager()
      :m_ioServiceAudio(true, _T("Audio Background Thread")), // with default work
       m_audioBackgroundTasks(m_ioServiceAudio.Get())
   {
      m_ioServiceAudio.Run();
   }

   /// dtor
   ~TaskManager()
   {
      m_ioServiceAudio.Join();
   }

private:
   /// io service thread for audio background tasks
   IoServiceThread m_ioServiceAudio;

   /// audio background tasks
   TaskGroup m_audioBackgroundTasks;
};
