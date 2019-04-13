//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file RenderContainer.hpp Render container
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include <set>

// forward references
class GraphicsTaskManager;
class IRenderable;
class RenderOptions;

/// \brief container for renderable objects
class RENDERENGINE_DECLSPEC RenderContainer
{
public:
   /// ctor
   RenderContainer(GraphicsTaskManager& taskManager);

   /// add renderable object
   void Add(std::shared_ptr<IRenderable> spRenderObject, unsigned int uiPriority);

   /// remove renderable object
   void Remove(std::shared_ptr<IRenderable> spRenderObject);

   /// render all objects
   void Render(RenderOptions& options);

private:
   // forward reference
   struct QueueItem;

   /// prepare object in background thread
   void AsyncPrepare(QueueItem item);

   /// upload object in OpenGL thread
   void AsyncUpload(QueueItem item);

   /// internal add function
   void InternalAdd(QueueItem item);

   /// internal remove function
   void InternalRemove(std::shared_ptr<IRenderable> spRenderObject);

private:
   /// task manager
   GraphicsTaskManager& m_taskManager;

   /// priority queue item
   struct QueueItem
   {
      /// ctor
      QueueItem(std::shared_ptr<IRenderable> spRenderable, unsigned int uiPriority)
         :m_uiPriority(uiPriority),
         m_spRenderable(spRenderable)
      {
      }

      /// queue priority
      unsigned int m_uiPriority;

      /// renderable object
      std::shared_ptr<IRenderable> m_spRenderable;

      /// less operator
      bool operator<(const QueueItem& rhs) const
      {
         if (m_uiPriority != rhs.m_uiPriority)
            return m_uiPriority < rhs.m_uiPriority;

         return m_spRenderable.get() < rhs.m_spRenderable.get();
      }
   };

   /// all render objects
   std::multiset<QueueItem> m_setRenderObjects;
};
