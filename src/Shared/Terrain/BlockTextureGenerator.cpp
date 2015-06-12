//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file BlockTextureGenerator.cpp Texture generator for terrain block
//
#pragma once

// includes
#include "stdafx.h"
#include "BlockTextureGenerator.hpp"
#include "FrameBuffer.hpp"
#include "OrthoCamera.hpp"
#include "Bitmap.hpp"
#include "TraceOutputStopwatch.hpp"
#include "HighResolutionTimer.hpp"

using namespace Terrain;

/// size for block texture
const unsigned int c_uiBlockTextureSize = 2048;

/// texture names for all 16 possible textures
static LPCTSTR g_pszTextureNames[16] =
{
   _T(""), _T(""), _T(""), _T(""),
   _T(""), _T(""), _T(""), _T(""),
   _T(""), _T(""), _T(""), _T(""),
   _T(""), _T(""), _T(""), _T("")
};

BlockTextureGenerator::BlockTextureGenerator()
{
}

BlockTextureGenerator::~BlockTextureGenerator()
{
}

void BlockTextureGenerator::Prepare()
{
}

void BlockTextureGenerator::Upload()
{
}

TexturePtr BlockTextureGenerator::Generate(std::shared_ptr<Model::DataBlock> spDataBlock)
{
#if 1
   TraceOutputStopwatch<HighResolutionTimer> stopwatch(_T("BlockTextureGenerator::Generate"));

   OpenGL::PushedAttributes attr(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_LINE_BIT | GL_POLYGON_BIT);

   TexturePtr spTexture;

   OpenGL::FrameBuffer buffer;
   buffer.Generate();
   buffer.Bind();

   spTexture.reset(new Texture);
   spTexture->Generate();
   spTexture->Bind();
   spTexture->UploadEmpty(c_uiBlockTextureSize, c_uiBlockTextureSize);

   buffer.SetTexture(spTexture);

   buffer.Bind();

   OpenGL::ViewportRestorer viewportRestorer;
   glViewport(0, 0, c_uiBlockTextureSize, c_uiBlockTextureSize);

   OrthoCamera camera(0.0, 1.0, 0.0, 1.0);
   camera.Use();

   glLoadIdentity();

   glDisable(GL_TEXTURE_2D);
   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_BUFFER);
   //glDisable(GL_CULL_FACE);

   //glClearColor(255, 255, 0, 255);
   //glClear(GL_COLOR_BUFFER_BIT);

   glBegin(GL_TRIANGLES);
      glColor3ub(0, 0, 0);       glVertex2d(0.0, 0.0);
      glColor3ub(255, 0, 255);   glVertex2d(1.0, 1.0);
      glColor3ub(0, 255, 0);     glVertex2d(1.0, 0.0);

      glColor3ub(0, 0, 0);       glVertex2d(0.0, 0.0);
      glColor3ub(0, 255, 128);   glVertex2d(0.0, 1.0);
      glColor3ub(255, 0, 255);   glVertex2d(1.0, 1.0);
   glEnd();

   RenderZoneTextures();

   RenderPaths();

   RenderDecals();

   //buffer.Unbind();

   return spTexture;

#else
   if (m_spDefaultTexture != nullptr)
      return m_spDefaultTexture;

   Bitmap bmp(c_uiBlockTextureSize, c_uiBlockTextureSize, Color(255, 0, 255));

   for (int x = 0; x < c_uiBlockTextureSize; x++)
      for (int y = 0; y < c_uiBlockTextureSize; y++)
         if ((x + y) & 1)
            bmp.Data()[y*c_uiBlockTextureSize + x] = Color(255, 255, 0);

   m_spDefaultTexture.reset(new Texture);
   m_spDefaultTexture->Generate();
   m_spDefaultTexture->Upload(bmp, false);

   return m_spDefaultTexture;
#endif
}

void BlockTextureGenerator::RenderZoneTextures()
{
}

void BlockTextureGenerator::RenderPaths()
{
}

void BlockTextureGenerator::RenderDecals()
{
}
