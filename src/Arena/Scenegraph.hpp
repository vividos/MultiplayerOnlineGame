//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ArenaScenegraph.hpp Arena scenegraph
//
#pragma once

// includes
#include "IScenegraph.hpp"
#include "RenderContainer.hpp"
#include "SkyRenderManager.hpp"
#include "Object.hpp"
#include <map>

// forward references
class RenderEngine;
class IFileSystem;
class GraphicsTaskManager;
class ModelRenderManager;

namespace Arena
{
class ViewModel;

/// \brief scenegraph for Arena
/// \details renders sky, checkerboard, player and mobiles from ArenaViewModel
class Scenegraph: public IScenegraph
{
public:
   /// ctor
   Scenegraph(ViewModel& viewModel, IFileSystem& fileSystem, GraphicsTaskManager& taskManager,
      RenderEngine& renderEngine);

   /// renders object; called every frame; called in thread with rendering context
   virtual void Render(RenderOptions& options) override;

   /// sets new LOD center position
   void SetPosition(const Vector3d& vPosition, double dAngleDir) throw();

private:
   /// a new mobile has been added
   void AddMobile(MobilePtr spMobile);

   /// a mobile has been removed
   void RemoveMobile(const ObjectId& id);

   /// movement info of mobile has been updated
   void UpdateMobile(const ObjectId& id, const MovementInfo& info);

private:
   /// inits mobiles to render
   void InitMobiles();

   /// inits player to render
   void InitPlayer();

   /// updates time on sky render manager
   void UpdateSkyRenderManager();

private:
   /// view model
   ViewModel& m_viewModel;

   /// render engine
   RenderEngine& m_renderEngine;

   /// render container with all render objects
   RenderContainer m_renderContainer;

   /// sky renderer
   std::shared_ptr<SkyRenderManager> m_spSkyRenderManager;

   /// model render manager
   std::shared_ptr<ModelRenderManager> m_spModelRenderManager;
};

} // namespace Arena
