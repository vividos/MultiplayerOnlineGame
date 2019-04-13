//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SkyRenderManager.hpp Sky render manager
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include "IRenderable.hpp"
#include "Vector3.hpp"
#include "Texture.hpp"
#include "MilkyWaySkyboxRenderer.hpp"
#include <ulib/DateTime.hpp>

// forward references
class GraphicsTaskManager;
class IFileSystem;

/// \brief sky render manager
/// \details renders sky, including:
/// * day colors, sunset, sunrise colors
/// * night stars
/// * sun, including position
/// * moon, including position
/// * fog levels
/// * procedural clouds, including weather type
/// \see http://www.gamedev.net/topic/86024-sky-rendering-techniques/
/// \see http://web.archive.org/web/20031011052035/http://www.geocities.com/ngdash/whitepapers/skydomecolor.html
class RENDERENGINE_DECLSPEC SkyRenderManager: public IRenderable
{
public:
   /// ctor
   SkyRenderManager(GraphicsTaskManager& taskManager, IFileSystem& fileSystem);
   /// dtor
   virtual ~SkyRenderManager() {}

   // set methods

   /// sets current date/time
   void SetCurrentDateTime(DateTime dtNow) { m_dtNow = dtNow; }

   // virtual methods from IRenderable

   /// returns if Prepare() call is needed (or it's a no-op)
   virtual bool IsPrepareNeeded() const override { return true; }

   /// returns if Upload() call is needed (or it's a no-op)
   virtual bool IsUploadNeeded() const override { return true; }

   /// prepares or calculates data; may be done on a worker thread
   virtual void Prepare() override;

   /// uploads data to graphics card; called in thread with rendering context
   virtual void Upload() override;

   /// renders object; called every frame; called in thread with rendering context
   virtual void Render(RenderOptions& options) override;

   /// cleans up data; called in thread with rendering context
   virtual void Done();

private:
   /// calculates position from given elevation and azimuth angles
   static Vector3d PosFromAngles(double dRadius, double dElevation, double dAzimuth);

   /// calculates sun direction, based on daytime
   void SunDirection(double& dElevation, double& dAzimuth);

   /// calculates moon direction, based on daytime
   void MoonDirection(double& dElevation, double& dAzimuth);

   // render night sky dome
   void RenderNightSky(RenderOptions& options);

   /// renders day sky
   void RenderDaySky();

   /// renders sun
   void RenderSun();

   /// renders moon
   void RenderMoon();

   /// renders triangle lines
   void RenderLines();

   // render sun position
   void RenderLinesSun();

   // render moon position
   void RenderLinesMoon();

private:
   /// current date/time
   DateTime m_dtNow;

   /// current direction view angle
   double m_dAngleDirection;

   /// current up view angle
   double m_dAngleUp;

   /// field of view in degrees
   double m_dFov;

   /// aspect ratio of screen
   double m_dRatio;

   /// night sky renderer
   MilkyWaySkyboxRenderer m_milkyWaySkyboxRenderer;

   /// texture loader
   std::shared_ptr<TextureLoader> m_spTextureLoader;

   /// texture for sky tones
   TexturePtr m_spTexSkyTonesMap;

   /// sun texture
   TexturePtr m_spTexSun;

   /// moon texture
   TexturePtr m_spTexMoon;
};
