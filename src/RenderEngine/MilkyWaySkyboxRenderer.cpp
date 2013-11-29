//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file MilkyWaySkyboxRenderer.cpp Milky way skybox renderer
//

// includes
#include "StdAfx.h"
#include "MilkyWaySkyboxRenderer.hpp"
#include "TextureLoader.hpp"
#include "GraphicsTaskManager.hpp"
#include "IFileSystem.hpp"
#include "OpenGL.hpp"

/// distance for skybox
const double c_dSkyboxDistance = 5000.0;

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
      { g_u1, g_vOffset*5.0 + g_v1 },
      { g_u0, g_vOffset*5.0 + g_v1 },
      { g_u0, g_vOffset*5.0 + g_v0 },
      { g_u1, g_vOffset*5.0 + g_v0 },
   },
   { // +Y
      { g_u1, g_vOffset*4.0 + g_v1 },
      { g_u0, g_vOffset*4.0 + g_v1 },
      { g_u0, g_vOffset*4.0 + g_v0 },
      { g_u1, g_vOffset*4.0 + g_v0 },
   },
   { // +Z
      { g_u0, g_vOffset*3.0 + g_v0 },
      { g_u1, g_vOffset*3.0 + g_v0 },
      { g_u1, g_vOffset*3.0 + g_v1 },
      { g_u0, g_vOffset*3.0 + g_v1 },
   },
   { // -X
      { g_u1, g_vOffset*2.0 + g_v1 },
      { g_u0, g_vOffset*2.0 + g_v1 },
      { g_u0, g_vOffset*2.0 + g_v0 },
      { g_u1, g_vOffset*2.0 + g_v0 },
   },
   {
      { g_u0, g_vOffset*1.0 + g_v1 },
      { g_u1, g_vOffset*1.0 + g_v1 },
      { g_u1, g_vOffset*1.0 + g_v0 },
      { g_u0, g_vOffset*1.0 + g_v0 },
   },
   {
      { g_u1, g_vOffset*0.0 + g_v1 },
      { g_u0, g_vOffset*0.0 + g_v1 },
      { g_u0, g_vOffset*0.0 + g_v0 },
      { g_u1, g_vOffset*0.0 + g_v0 },
   },
};


MilkyWaySkyboxRenderer::MilkyWaySkyboxRenderer(GraphicsTaskManager& taskManager, IFileSystem& fileSystem) throw()
:m_taskManager(taskManager),
 m_fileSystem(fileSystem)
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

void MilkyWaySkyboxRenderer::Prepare()
{
   // load texture
   m_spTexture.reset(new Texture);

   m_spTextureLoader.reset(new TextureLoader(m_taskManager, m_fileSystem));
   m_spTextureLoader->Load(_T("sky\\eso0932a-texcubes.jpg"), m_spTexture, false);
}

void MilkyWaySkyboxRenderer::Render(RenderOptions& /*options*/)
{
   OpenGL::PushedAttributes attrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_POLYGON_BIT);

   // enable needed states
   glDisable(GL_DEPTH_TEST);

   glPushMatrix();
   glLoadIdentity();

   glColor3f(1.0f, 1.0f, 1.0f);
   glPolygonMode(GL_FRONT, GL_FILL);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   m_spTexture->Bind();

   glColor3ub(255, 255, 255);

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

   glPopMatrix();

   OpenGL::CountPolygons(6*4*2);
}

void MilkyWaySkyboxRenderer::Done()
{
   m_spTexture.reset();
}
