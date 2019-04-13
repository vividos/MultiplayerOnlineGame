//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MilkyWaySkyboxRenderer.cpp Milky way skybox renderer
//

// includes
#include "StdAfx.h"
#include "MilkyWaySkyboxRenderer.hpp"
#include "TextureLoader.hpp"
#include "GraphicsTaskManager.hpp"
#include "IFileSystem.hpp"
#include "OpenGL.hpp"
#include "RenderOptions.hpp"
#include <ulib/DateTime.hpp>

/// distance for skybox
const double c_dSkyboxDistance = 100.0;

static unsigned int g_auiCoordIndices[6][4] =
{
   // tex order: +X, +Y, +Z, -X, -Y and -Z
   {2, 1, 0, 3}, // top
   {5, 6, 7, 4}, // bottom
   {5, 1, 2, 6}, // right
   {0, 4, 7, 3}, // left
   {5, 4, 0, 1}, // front
   {6, 2, 3, 7}, // back
};

static double g_adVertexCoords[8][3] =
{
   { -1.0,  1.0,  1.0 },
   {  1.0,  1.0,  1.0 },
   {  1.0,  1.0, -1.0 },
   { -1.0,  1.0, -1.0 },

   
   { -1.0, -1.0,  1.0 },
   {  1.0, -1.0,  1.0 },
   {  1.0, -1.0, -1.0 },
   { -1.0, -1.0, -1.0 }
};

static double
   g_u0 = 0.5/256.0, // left
   g_u1 = (256.0-0.50)/256.0, // right
   g_v0 = g_u0,
   g_v1 = g_u1 / 6.0,
   g_vOffset = 1.0/6.0;


static GLdouble g_adTexCoords[6][4][2] =
{
   // order of tiles: +X, +Y, +Z, -X, -Y and -Z

   // +X
   {
      { g_u1, g_vOffset*0.0 + g_v0 },
      { g_u0, g_vOffset*0.0 + g_v0 },
      { g_u0, g_vOffset*0.0 + g_v1 },
      { g_u1, g_vOffset*0.0 + g_v1 },
   },
   { // +Y
      { g_u1, g_vOffset*1.0 + g_v0 },
      { g_u0, g_vOffset*1.0 + g_v0 },
      { g_u0, g_vOffset*1.0 + g_v1 },
      { g_u1, g_vOffset*1.0 + g_v1 },
   },
   { // +Z
      { g_u0, g_vOffset*2.0 + g_v1 },
      { g_u1, g_vOffset*2.0 + g_v1 },
      { g_u1, g_vOffset*2.0 + g_v0 },
      { g_u0, g_vOffset*2.0 + g_v0 },
   },
   { // -X
      { g_u1, g_vOffset*3.0 + g_v0 },
      { g_u0, g_vOffset*3.0 + g_v0 },
      { g_u0, g_vOffset*3.0 + g_v1 },
      { g_u1, g_vOffset*3.0 + g_v1 },
   },
   {
      { g_u0, g_vOffset*4.0 + g_v0 },
      { g_u1, g_vOffset*4.0 + g_v0 },
      { g_u1, g_vOffset*4.0 + g_v1 },
      { g_u0, g_vOffset*4.0 + g_v1 },
   },
   {
      { g_u1, g_vOffset*5.0 + g_v0 },
      { g_u0, g_vOffset*5.0 + g_v0 },
      { g_u0, g_vOffset*5.0 + g_v1 },
      { g_u1, g_vOffset*5.0 + g_v1 },
   },
};


MilkyWaySkyboxRenderer::MilkyWaySkyboxRenderer(GraphicsTaskManager& taskManager, IFileSystem& fileSystem)
:m_taskManager(taskManager),
 m_fileSystem(fileSystem),
 m_dRotateAngle(0.0)
{
   // top vertices
   for (unsigned int i=0; i<6*4; i++)
   {
      unsigned int uiIndex = g_auiCoordIndices[i/4][i&3];
      m_avSkyboxCoords[i] = c_dSkyboxDistance * Vector3d(
         g_adVertexCoords[uiIndex][0],
         g_adVertexCoords[uiIndex][1],
         g_adVertexCoords[uiIndex][2]);
   }
}

void MilkyWaySkyboxRenderer::SetCurrentDateTime(const DateTime& dtNow)
{
   // calculate current angle
   unsigned int uiDaytimeSeconds = (dtNow.Hour() * 60 + dtNow.Minute()) * 60 + dtNow.Second();
   m_dRotateAngle = AngleInRange(uiDaytimeSeconds / 24.0 / 60.0 / 60.0 * 360.0);
}

void MilkyWaySkyboxRenderer::Prepare()
{
   // load texture
   m_spTexture.reset(new Texture);

   m_spTextureLoader.reset(new TextureLoader(m_taskManager, m_fileSystem));
   m_spTextureLoader->Load(_T("sky\\eso0932a-texcubes.jpg"), m_spTexture, false);
}

void MilkyWaySkyboxRenderer::Render(RenderOptions& /*options*/)
{
   // note: we assume here that our caller already changed necessary OpenGL states; saves
   // some state changes here

   glPushMatrix();

   // rotate sky around polaris
   // lucky for us, the star lies where x axis meets our box
   Vector3d vPolaris(1, 0, 0);

   glRotated(m_dRotateAngle, vPolaris.X(), vPolaris.Y(), vPolaris.Z());

   const double c_dEclipticGalacticDiff = 40.0;
   glRotated(c_dEclipticGalacticDiff, 0, 0, 1.0);

   glColor4ub(255, 255, 255, 255);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   m_spTexture->Bind();

   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_DOUBLE, 0, m_avSkyboxCoords[0].Data());

   glEnableClientState(GL_INDEX_ARRAY);
   glIndexPointer(GL_INT, 0, &g_auiCoordIndices[0]);

   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glTexCoordPointer(2, GL_DOUBLE, 0, &g_adTexCoords[0]);

   for (unsigned int i=0; i<6; i++)
      glDrawArrays(GL_QUADS, i*4, 4);

   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   glDisableClientState(GL_INDEX_ARRAY);

   OpenGL::CountPolygons(6*4*2);

   glPopMatrix();
}

void MilkyWaySkyboxRenderer::Done()
{
   m_spTexture.reset();
}
