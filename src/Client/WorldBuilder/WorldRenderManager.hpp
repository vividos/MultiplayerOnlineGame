//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file WorldRenderManager.hpp World render manager
//
#pragma once

// includes
#include "IScenegraph.hpp"
#include "GraphicsTaskManager.hpp"

// forward references
class RenderEngine;
class PerspectiveCamera;
class WorldGenerator;
class PolygonGraphRenderer;

/// world render manager
class WorldRenderManager: public IScenegraph
{
public:
   /// ctor
   WorldRenderManager(RenderEngine& renderEngine, WorldGenerator& worldGenerator);
   /// dtor
   ~WorldRenderManager() throw();

   /// returns camera
   std::shared_ptr<PerspectiveCamera> GetCamera() { return m_spCamera; }

   /// world view modes
   typedef enum T_enWorldViewMode
   {
      worldViewNone = 0,      ///< no view
      worldViewPolygonGraph,  ///< shows polygon graph
   };

   /// sets new world view mode
   void SetWorldViewMode(T_enWorldViewMode enWorldViewMode);

private:
   /// renders world
   virtual void Render(RenderOptions& options) override;

   void RenderOutline();

private:
   /// render engine
   RenderEngine& m_renderEngine;

   /// camera
   std::shared_ptr<PerspectiveCamera> m_spCamera;

   /// world generator
   WorldGenerator& m_worldGenerator;

   /// task manager
   GraphicsTaskManager m_taskManager;

   /// current world view mode
   T_enWorldViewMode m_enWorldViewMode;

   /// renderer for polygon graph
   std::unique_ptr<PolygonGraphRenderer> m_upPolygonGraphRenderer;
};
