//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Renderer/Renderer.hpp Underworld renderer
//
#pragma once

// includes
#include "stdafx.h"
#include "TextureAtlas.hpp"
#include "IScenegraph.hpp"

// forward references
class IFileSystem;
class RenderWindow;
class LevelRenderer;
class GraphicsTaskManager;

namespace Underworld
{
class Level;
}

/// Undewrorld renderer
class Renderer: public IScenegraph
{
public:
   /// ctor
   Renderer(IFileSystem& fileSystem)
      :m_fileSystem(fileSystem)
   {
   }
   /// dtor
   ~Renderer()
   {
   }

   /// starts async loading of new level
   void AsyncLoadNewLevel(GraphicsTaskManager& graphicsTaskManager, const Underworld::Level& level);

   /// loads new level
   //void LoadNewLevel(const Underworld::Level& level);

   // virtual methods from IScenegraph
   virtual bool IsPrepareNeeded() const override { return true; }
   virtual bool IsUploadNeeded() const override { return true; }
   virtual void Prepare() override;
   virtual void Upload() override;
   virtual void Render(RenderOptions& options) override;
   virtual void Done() override;

private:
   void PrepareLevel(GraphicsTaskManager& graphicsTaskManager, const Underworld::Level& level);

   void UploadLevel(std::shared_ptr<LevelRenderer> spLevelRenderer);

private:
   /// file system
   IFileSystem& m_fileSystem;

   /// texture atlas
   TextureAtlas m_atlas;

   /// level renderer
   std::shared_ptr<LevelRenderer> m_spLevelRenderer;
};
