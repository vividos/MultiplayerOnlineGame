//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/Renderer/Renderer.cpp Underworld renderer
//

// includes
#include "stdafx.h"
#include "Renderer.hpp"
#include "RenderWindow.hpp"
#include "RenderOptions.hpp"
#include "Underworld.hpp"
#include "Texture.hpp"
#include "TextureParameter.hpp"
#include "OpenGL.hpp"
#include "LevelRenderer.hpp"
#include "GraphicsTaskManager.hpp"

using namespace Underworld;

void Renderer::AsyncLoadNewLevel(GraphicsTaskManager& graphicsTaskManager, const Level& level)
{
   graphicsTaskManager.BackgroundTaskGroup().Add(
      std::bind(&Renderer::PrepareLevel, this, std::ref(graphicsTaskManager), std::cref(level)));
}

void Renderer::PrepareLevel(GraphicsTaskManager& graphicsTaskManager, const Underworld::Level& level)
{
   std::shared_ptr<LevelRenderer> spLevelRenderer(new LevelRenderer);
   spLevelRenderer->Prepare(m_atlas, level);

   graphicsTaskManager.UploadTaskGroup().Add(
      std::bind(&Renderer::UploadLevel, this, spLevelRenderer));
}

void Renderer::UploadLevel(std::shared_ptr<LevelRenderer> spLevelRenderer)
{
   spLevelRenderer->Upload();

   // swap renderer (an atomic operation)
   m_spLevelRenderer = spLevelRenderer;
}

void Renderer::Prepare()
{
   m_atlas.Load(m_fileSystem);
}

void Renderer::Upload()
{
   m_atlas.Generate();
}

void Renderer::Render(RenderOptions& /*options*/)
{
   OpenGL::RenderXyzAxes();

   if (m_spLevelRenderer == nullptr)
      return;

   glColor4ub(255, 255, 255, 255);
   glEnable(GL_TEXTURE_2D);
   glPolygonMode(GL_FRONT, GL_FILL);
   glPolygonMode(GL_BACK, GL_LINE);
   glDisable(GL_CULL_FACE);
   glCullFace(GL_FRONT_AND_BACK);

   m_atlas.GetTexture()->Bind();

   TextureParameter<GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST, GL_MODULATE>::Use();

   if (m_spLevelRenderer != nullptr)
      m_spLevelRenderer->Render();
}

void Renderer::Done()
{
   m_spLevelRenderer.reset();

   m_atlas.Done();
}
