//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file WorldRenderManager.cpp World render manager
//

// includes
#include "stdafx.h"
#include "WorldRenderManager.hpp"
#include "WorldGenerator.hpp"
#include "PolygonGraphRenderer.hpp"

WorldRenderManager::WorldRenderManager(RenderEngine& renderEngine, WorldGenerator& worldGenerator)
:m_renderEngine(renderEngine),
 m_worldGenerator(worldGenerator),
 m_enWorldViewMode(worldViewNone)
{
}

WorldRenderManager::~WorldRenderManager() throw()
{
}

void WorldRenderManager::SetWorldViewMode(T_enWorldViewMode enWorldViewMode)
{
   if (m_enWorldViewMode == enWorldViewMode)
      return;

   switch (enWorldViewMode)
   {
   case worldViewNone:
      break;

   case worldViewPolygonGraph:
      if (m_upPolygonGraphRenderer == nullptr)
         m_upPolygonGraphRenderer.reset(new PolygonGraphRenderer(m_worldGenerator.GetGraph()));
      m_upPolygonGraphRenderer->Prepare();
      m_upPolygonGraphRenderer->Upload();
      break;

   default:
      ATLASSERT(false);
      break;
   }
}

void WorldRenderManager::Render(RenderOptions& options)
{
   m_taskManager.UploadOne();

   switch (m_enWorldViewMode)
   {
   case worldViewNone:
      //m_worldGenerator;
      break;

   case worldViewPolygonGraph:
      if (m_upPolygonGraphRenderer != nullptr)
         m_upPolygonGraphRenderer->Render(options);
      break;

   default:
      ATLASSERT(false);
      break;
   }
}
