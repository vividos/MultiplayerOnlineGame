//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TerrainViewerView.hpp View for TerrainViewer
//
#pragma once

// includes
#include "TerrainViewerView.hpp"
#include "IView.hpp"
#include "IScenegraph.hpp"
#include "RenderContainer.hpp"
#include "Model/DataBlockManager.hpp"
#include "View/TerrainRenderManager.hpp"
#include "SkyRenderManager.hpp"
#include "LightingManager.hpp"
#include "ModelRenderManager.hpp"
#include "Player.hpp"

// forward references
class IFileSystem;
class RenderEngine;
class Vector3d;
class PerspectiveCamera;

namespace TerrainViewer
{

/// \brief terrain view
/// \details view / scenegraph that shows terrain
class View:
   public IView,
   public IScenegraph
{
public:
   /// ctor
   View(RenderEngine& engine, GraphicsTaskManager& taskManager,
      IFileSystem& fileSystem);
   /// dtor
   virtual ~View() throw(){}

   // get methods

   /// returns camera
   std::shared_ptr<PerspectiveCamera> GetCamera() { return m_spCamera; }

   /// returns fill mode
   bool FillMode() const throw() { return m_bFillMode; }

   // set methods

   /// sets fill mode
   void FillMode(bool bFillMode) throw() { m_bFillMode = bFillMode; }

private:
   // virtual methods from IView

   /// called before rendering view
   virtual void PreRender();

   // virtual methods from IScenegraph

   /// renders terrain
   virtual void Render(RenderOptions& options) override;

private:
   /// render engine
   RenderEngine& m_engine;

   /// render container
   RenderContainer m_renderContainer;

   /// camera
   std::shared_ptr<PerspectiveCamera> m_spCamera;

   // terrain stuff

   /// data source
   std::shared_ptr<Terrain::IDataSource> m_spDataSource;

   /// data block manager
   Terrain::Model::DataBlockManager m_dataBlockManager;

   /// terrain renderer
   std::shared_ptr<Terrain::View::TerrainRenderManager> m_spTerrainRenderer;


   /// lighting manager
   OpenGL::LightingManager m_lightingManager;

   /// player
   Player m_player;

   /// model render manager
   std::shared_ptr<ModelRenderManager> m_spModelRenderManager;

   /// fill mode
   bool m_bFillMode;
};

} // namespace TerrainViewer
