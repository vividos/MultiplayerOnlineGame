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
#include "RenderEngine.hpp"
#include "PerspectiveCamera.hpp"

WorldRenderManager::WorldRenderManager(RenderEngine& renderEngine, WorldGenerator& worldGenerator)
:m_renderEngine(renderEngine),
 m_spCamera(new PerspectiveCamera),
 m_worldGenerator(worldGenerator),
 m_enWorldViewMode(worldViewNone)
{
   m_renderEngine.SetCamera(m_spCamera);
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

      // upload render data
      m_taskManager.UploadTaskGroup().Add(
         std::bind(&PolygonGraphRenderer::Upload, m_upPolygonGraphRenderer.get()));
      break;

   default:
      ATLASSERT(false);
      break;
   }

   m_enWorldViewMode = enWorldViewMode;
}

void WorldRenderManager::Render(RenderOptions& options)
{
   m_taskManager.UploadOne();

   switch (m_enWorldViewMode)
   {
   case worldViewNone:
      RenderOutline();
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

void WorldRenderManager::RenderOutline()
{
   OpenGL::PushedAttributes attrib(GL_LINE_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT);

   glDisable(GL_TEXTURE_2D);
   glDisable(GL_LIGHTING);

   glLineWidth(3.0f);

   glColor3ub(255, 255, 255);
   glBegin(GL_LINE_LOOP);
   glVertex3d(0.0, 0.0, 0.0);
   glVertex3d(1.0, 0.0, 0.0);
   glVertex3d(0.0, 1.0, 0.0);
   glEnd();
}
