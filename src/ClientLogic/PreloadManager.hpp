//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file PreloadManager.hpp Pre-load manager
//
#pragma once

// includes
#include "ClientLogic.hpp"
#include <atomic>

// forward references
class GraphicsTaskManager;

/// manages pre-loading assets before game starts
class CLIENTLOGIC_DECLSPEC PreloadManager
{
public:
   /// background task type
   typedef std::function<void()> T_fnBackgroundTask;

   /// function to signal preloading has finished
   typedef std::function<void()> T_fnFinishedPreload;

   /// ctor
   PreloadManager(GraphicsTaskManager& taskManager)
      :m_taskManager(taskManager)
   {
   }

   /// returns percent of preloading already done
   unsigned int PercentDone() const;

   /// adds task
   void AddBackgroundTask(T_fnBackgroundTask fnTask);

   /// finishes task queue
   void FinishQueue(T_fnFinishedPreload fnFinishedPreload);

private:
   /// executes task
   void ExecTask(T_fnBackgroundTask fnTask);

   /// finishes background queue
   void AsyncFinishBackgroundQueue(T_fnFinishedPreload fnFinishedPreload);

   /// finishes upload queue
   void AsyncFinishUploadQueue(T_fnFinishedPreload fnFinishedPreload);

private:
   /// task manager
   GraphicsTaskManager& m_taskManager;

   /// callback to signal preloading has finished
   T_fnFinishedPreload m_fnFinishedPreload;

   /// number of tasks added
   unsigned int m_uiNumTasks;

   /// number of tasks done
   std::atomic<unsigned int> m_uiTasksDone;
};
