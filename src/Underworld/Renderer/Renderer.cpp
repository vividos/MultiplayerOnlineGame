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

using namespace Underworld;

void Renderer::Load(IFileSystem& fileSystem)
{
   m_atlas.Load(fileSystem);
}

void Renderer::LoadNewLevel(const Underworld::Level& level)
{
   std::shared_ptr<LevelRenderer> spLevelRenderer(new LevelRenderer);
   spLevelRenderer->Prepare(m_atlas, level);

   spLevelRenderer->Upload();

   m_spLevelRenderer = spLevelRenderer;
}

void Renderer::Upload()
{
   RenderOptions renderOptions;
   renderOptions.Set(RenderOptions::optionCullBackface, false);
   renderOptions.Set(RenderOptions::optionBackfaceAsLines, true);

   m_atlas.Generate();
}

void Renderer::Render(RenderOptions& /*options*/)
{
   glDisable(GL_TEXTURE_2D);

   OpenGL::RenderXyzAxes();

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
