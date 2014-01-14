//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file PreloadManager.cpp Pre-load manager
//

// includes
#include "stdafx.h"
#include "PreloadManager.hpp"
#include "GraphicsTaskManager.hpp"

unsigned int PreloadManager::PercentDone() const
{
   return m_uiTasksDone / m_uiNumTasks;
}

void PreloadManager::AddBackgroundTask(T_fnBackgroundTask fnTask)
{
   m_taskManager.BackgroundTaskGroup().Add(
      std::bind(&PreloadManager::ExecTask, this, fnTask));

   m_uiNumTasks++;
}

void PreloadManager::FinishQueue(T_fnFinishedPreload fnFinishedPreload)
{
   // add last background task
   AddBackgroundTask(
      std::bind(&PreloadManager::AsyncFinishBackgroundQueue, this, fnFinishedPreload));
}

void PreloadManager::ExecTask(T_fnBackgroundTask fnTask)
{
   fnTask();

   m_uiTasksDone++;
}

void PreloadManager::AsyncFinishBackgroundQueue(T_fnFinishedPreload fnFinishedPreload)
{
   m_taskManager.UploadTaskGroup().Add(
      std::bind(&PreloadManager::AsyncFinishUploadQueue, this, fnFinishedPreload));
}

void PreloadManager::AsyncFinishUploadQueue(T_fnFinishedPreload fnFinishedPreload)
{
   fnFinishedPreload();
}
