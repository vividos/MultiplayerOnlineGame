//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file MilkyWaySkyboxRenderer.hpp Milky way skybox renderer
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include "Vector3d.hpp"
#include "Texture.hpp"
#include "IRenderable.hpp"

// forward references
class GraphicsTaskManager;
class IFileSystem;
class TextureLoader;
class RenderOptions;
class DateTime;

/// renderer for skybox
class RENDERENGINE_DECLSPEC MilkyWaySkyboxRenderer: public IRenderable
{
public:
   /// ctor
   MilkyWaySkyboxRenderer(GraphicsTaskManager& taskManager, IFileSystem& fileSystem) throw();
   /// dtor
   virtual ~MilkyWaySkyboxRenderer() throw() {}

   /// sets current date/time
   void SetCurrentDateTime(const DateTime& dtNow);

   // virtual methods from IRenderable

   /// Prepare() call is needed
   virtual bool IsPrepareNeeded() const override { return true; }

   /// Upload() call is needed
   virtual bool IsUploadNeeded() const override { return false; }

   /// loads skybox texture
   virtual void Prepare() override;

   /// uploads texture
   virtual void Upload() override {}

   /// renders skybox
   virtual void Render(RenderOptions& options) override;

   /// cleans up data; called in thread with rendering context
   virtual void Done() override;

private:
   /// task manager
   GraphicsTaskManager& m_taskManager;

   /// file system
   IFileSystem& m_fileSystem;

   /// texture loader
   std::shared_ptr<TextureLoader> m_spTextureLoader;

   /// texture for skybox
   TexturePtr m_spTexture;

   /// coordinate array for skybox
   Vector3d m_avSkyboxCoords[24];

   /// current rotate angle
   double m_dRotateAngle;
};
