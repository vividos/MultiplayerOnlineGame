//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file RenderContainer.hpp Render container
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include <vector>

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
   void Add(std::shared_ptr<IRenderable> spRenderObject);

   /// remove renderable object
   void Remove(std::shared_ptr<IRenderable> spRenderObject);

   /// render all objects
   void Render(RenderOptions& options);

private:
   /// prepare object in background thread
   void AsyncPrepare(std::shared_ptr<IRenderable> spRenderObject);

   /// upload object in OpenGL thread
   void AsyncUpload(std::shared_ptr<IRenderable> spRenderObject);

   /// internal add function
   void InternalAdd(std::shared_ptr<IRenderable> spRenderObject);

   /// internal remove function
   void InternalRemove(std::shared_ptr<IRenderable> spRenderObject);

private:
   /// task manager
   GraphicsTaskManager& m_taskManager;

   /// all render objects
   std::vector<std::shared_ptr<IRenderable>> m_vecRenderObjects;
};
