//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/Renderer/Renderer.hpp Underworld renderer
//

// includes
#include "stdafx.h"
#include "TextureAtlas.hpp"
#include "IScenegraph.hpp"

// forward references
class IFileSystem;
class RenderWindow;
class LevelRenderer;

namespace Underworld
{
class Level;
}

/// Undewrorld renderer
class Renderer: public IScenegraph
{
public:
   /// ctor
   Renderer()
   {
   }
   /// dtor
   ~Renderer()
   {
   }

   /// loads renderer data
   void Load(IFileSystem& fileSystem);

   /// loads new level
   void LoadNewLevel(const Underworld::Level& level);

   // virtual methods from IScenegraph
   virtual bool IsPrepareNeeded() const override { return false; }
   virtual bool IsUploadNeeded() const override { return true; }
   //virtual void Prepare() override;
   virtual void Upload() override;
   virtual void Render(RenderOptions& options) override;
   virtual void Done() override;

private:
   /// texture atlas
   TextureAtlas m_atlas;

   /// level renderer
   std::shared_ptr<LevelRenderer> m_spLevelRenderer;
};
