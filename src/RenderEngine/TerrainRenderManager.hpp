//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TerrainRenderManager.hpp Terrain render manager
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include "IRenderable.hpp"

/// forward references
class IFileSystem;
class GraphicsTaskManager;

/// terrain render manager
class RENDERENGINE_DECLSPEC TerrainRenderManager: public IRenderable
{
public:
   /// ctor
   TerrainRenderManager(IFileSystem& fileSystem, GraphicsTaskManager& taskManager) throw();
   /// dtor
   virtual ~TerrainRenderManager() throw() {}

   /// sets new camera position (for LOD calculation)
   void SetPosition(const Vector3d& vPos, double dAngleDir);

   // virtual methods from IRenderable

   /// returns if Prepare() call is needed (or it's a no-op)
   virtual bool IsPrepareNeeded() const override { return false; }

   /// returns if Upload() call is needed (or it's a no-op)
   virtual bool IsUploadNeeded() const override { return false; }

   /// prepares or calculates data; may be done on a worker thread
   virtual void Prepare() override {}

   /// uploads data to graphics card; called in thread with rendering context
   virtual void Upload() override {}

   /// renders object; called every frame; called in thread with rendering context
   virtual void Render(RenderOptions& options) override;

   /// cleans up data; called in thread with rendering context
   virtual void Done() override;

private:
};
