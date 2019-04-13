//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file GraphicsTaskManager.cpp Graphics background task manager
//

// includes
#include "StdAfx.h"
#include "GraphicsTaskManager.hpp"
#include "OpenGL.hpp"

GraphicsTaskManager::GraphicsTaskManager()
:m_ioServiceBackgroundThread(true, _T("GraphicsTaskManager Background Thread")), // with default work
 m_backgroundTasks(m_ioServiceBackgroundThread.Get()),
 m_uploadTasks(m_ioServiceUploads)
{
   m_ioServiceBackgroundThread.Run();
}

GraphicsTaskManager::~GraphicsTaskManager()
{
   Cancel();

   m_ioServiceBackgroundThread.Join();
}

size_t GraphicsTaskManager::UploadOne()
{
   ATLASSERT(OpenGL::IsRenderThread() == true); // must be called in render thread

   return m_ioServiceUploads.poll_one();
}

void GraphicsTaskManager::Cancel()
{
   ATLTRACE(_T("GraphicsTaskManager::Cancel()\n"));

   BackgroundTaskGroup().Cancel();
   UploadTaskGroup().Clear();
}

void GraphicsTaskManager::Join()
{
   ATLTRACE(_T("GraphicsTaskManager::Join()\n"));

   m_ioServiceBackgroundThread.Join();
}
