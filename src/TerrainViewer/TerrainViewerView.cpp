//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TerrainViewerView.cpp View for TerrainViewer
//

// includes
#include "StdAfx.h"
#include "TerrainViewerView.hpp"
#include "RenderEngine.hpp"
#include "RenderOptions.hpp"
#include "ModelRenderInstance.hpp"
#include "ViewFrustum3d.hpp"
#include "PerspectiveCamera.hpp"
#include "DataSource/MandelbrotTerrainDataSource.hpp"
#include "DataSource/SineCosineTerrainDataSource.hpp"
#include "OverviewController.hpp"

TerrainViewer::View::View(RenderEngine& engine, GraphicsTaskManager& taskManager, IFileSystem& fileSystem)
:m_engine(engine),
 m_spCamera(new PerspectiveCamera),
 m_renderContainer(taskManager),
// m_spDataSource(new Terrain::MandelbrotTerrainDataSource),
 m_spDataSource(new Terrain::SineCosineTerrainDataSource),
 m_dataBlockManager(taskManager, m_spDataSource),
 m_spTerrainRenderer(new Terrain::View::TerrainRenderManager(engine, taskManager, m_dataBlockManager)),
 m_player(ObjectId::Null()),
 m_spModelRenderManager(new ModelRenderManager(fileSystem, taskManager)),
 m_bFillMode(false)
{
   // init camera
   m_spCamera->SetNearFarPlaneDistance(1.0, 10000.0);
   engine.SetCamera(m_spCamera);

   // init player
   m_player.Init(Vector3d(0.0, 0.0, 0.0));
   m_spModelRenderManager->InitPlayer(m_player);

   // init lighting
   OpenGL::LightSource ls(Color::White(), m_spCamera->GetPosition());
   m_lightingManager.Add(ls);
   m_lightingManager.ColorTracking(true);

   m_renderContainer.Add(m_spTerrainRenderer);
}

void TerrainViewer::View::PreRender()
{
   Vector3d vPlayerPos = m_player.Pos();

   if (m_dataBlockManager.IsAvailPos(vPlayerPos.X(), vPlayerPos.Z()))
   {
      double y = m_dataBlockManager.Height(vPlayerPos.X(), vPlayerPos.Z());
      vPlayerPos.Y(y);

      m_player.Pos(vPlayerPos);
   }

   m_spTerrainRenderer->SetPosition(vPlayerPos, m_player.ViewAngle());

   m_spModelRenderManager->UpdatePlayer(m_player);
}

void TerrainViewer::View::Render(RenderOptions& options)
{
   glColor3f(1.0f, 1.0f, 1.0f);

   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   glFrontFace(GL_CCW);

   // update light source pos
   {
      Vector3d vPlayerPos = m_player.Pos();

      OpenGL::LightSource& ls = m_lightingManager.Source(0);
      ls.Pos(vPlayerPos + Vector3d(0.0, 20.0, 0.0));
   }

   // render terrain
   {
      options.Set(RenderOptions::optionTerrainFilled, m_bFillMode);

      m_lightingManager.Enable();

      glDisable(GL_TEXTURE_2D);
      glColor3ub(255, 255, 0);

      m_spTerrainRenderer->Render(options);

      m_lightingManager.Disable();
   }

   m_spModelRenderManager->Render(options);

   // render view frustum
   {
      if (options.Get(RenderOptions::optionRenderViewFrustum))
      {
         glColor3ub(255, 255, 0);

         ViewFrustum3d::RenderLines(
            m_player.Pos(),
            m_player.ViewAngle(),
            0.0,
            75.0,
            1.3333333,
            1.0, 1000.0);
      }
   }
}

//void TerrainViewer::View::SetCameraPosition(const Vector3d& vPos, double dViewAngleDirection, double dViewAngleUp)
//{
//   Vector3d vCameraPos = m_player.Pos();
//   vCameraPos += vPos;
//
//   m_spCamera->SetPos(vCameraPos, dViewAngleDirection, dViewAngleUp);
//}
