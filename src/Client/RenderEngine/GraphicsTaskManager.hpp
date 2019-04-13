//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file GraphicsTaskManager.hpp Graphics background task manager
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include "TaskGroup.hpp"
#include "IoServiceThread.hpp"

/// \brief manager for background graphics tasks
/// \details provides two task groups, one for background tasks, e.g. texture
/// loading, etc., and one for upload tasks that need to use the current
/// OpenGL rendering context.
class RENDERENGINE_DECLSPEC GraphicsTaskManager
{
public:
   DEFINE_INSTANCE(GraphicsTaskManager)

   /// ctor
   GraphicsTaskManager();
   /// dtor
   ~GraphicsTaskManager();

   // get methods

   /// returns background task group
   TaskGroup& BackgroundTaskGroup() { return m_backgroundTasks; }

   /// returns upload task group
   TaskGroup& UploadTaskGroup() { return m_uploadTasks; }

   // actions

   /// uploads one element that was prepared in the background
   size_t UploadOne();

   /// cancels all outstanding operations
   void Cancel();

   /// join all worker threads
   void Join();

private:
   /// thread for background graphics tasks
   IoServiceThread m_ioServiceBackgroundThread;

   /// background graphics tasks
   TaskGroup m_backgroundTasks;

   /// io service for uploads to graphics card
   boost::asio::io_service m_ioServiceUploads;

   /// upload tasks
   TaskGroup m_uploadTasks;
};
