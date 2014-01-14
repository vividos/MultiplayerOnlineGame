//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ModelRenderManager.hpp Model render manager
//
#pragma once

// includes
#include "Model3DCommon.hpp"
#include "IRenderable.hpp"
#include "Mobile.hpp"
#include "MilkShape3D\ModelManager.hpp"
#include <map>

/// forward references
class IFileSystem;
class GraphicsTaskManager;
class Player;
class ModelRenderInstance;
class ObjectMap;

/// \brief model render manager
/// \details manages rendering player and zero or more mobiles
class MODEL3D_DECLSPEC ModelRenderManager: public IRenderable
{
public:
   /// ctor
   ModelRenderManager(IFileSystem& fileSystem, GraphicsTaskManager& taskManager) throw();
   /// dtor
   virtual ~ModelRenderManager() throw() {}

   /// inits player model
   void InitPlayer(const Player& player);

   /// updates player model
   void UpdatePlayer(const Player& player);

   /// returns access to model render instance for player (needed in ModelViewer)
   ModelRenderInstance& GetPlayerRenderInstance();

   /// adds mobile model
   void AddMobile(MobilePtr spMobile);

   /// removes mobile model
   void RemoveMobile(const ObjectId& id);

   /// updates mobile model
   void UpdateMobile(const ObjectId& id, const MovementInfo& info);

   /// updates all mobiles to render
   void UpdateMobilesToRender(const ObjectMap& objectMap);

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
   /// renders player
   void RenderPlayer(RenderOptions& renderOptions);

   /// renders all mobiles
   void RenderMobiles(RenderOptions& renderOptions);

private:
   /// model manager
   MilkShape3D::ModelManager m_modelManager;

   /// player model
   std::shared_ptr<ModelRenderInstance> m_spPlayerModelRenderInstance;

   /// selected object id
   ObjectId m_selectedObjectId;

   /// map type of object id to render instance
   typedef std::map<ObjectId, std::shared_ptr<ModelRenderInstance>> TMapMobileObjectRenderInstances;

   /// render instances for all mobile objects
   TMapMobileObjectRenderInstances m_mapMobileObjectRenderInstances;
};
