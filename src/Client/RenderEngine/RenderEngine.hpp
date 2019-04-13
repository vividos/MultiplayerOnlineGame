//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file RenderEngine.hpp Render engine class
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"

// forward references
class IScenegraph;
class ICamera;
class RenderOptions;
class RenderWindow;
typedef struct SDL_Window SDL_Window;

/// \brief render engine
class RENDERENGINE_DECLSPEC RenderEngine: public boost::noncopyable
{
public:
   DEFINE_INSTANCE(RenderEngine)

   /// ctor, creating a new window
   RenderEngine(const CString& cszWindowCaption, unsigned int uiWidth, unsigned int uiHeight, bool bFullscreen);
   /// ctor, accepting an already created SDL window
   RenderEngine(std::shared_ptr<SDL_Window> spWindow);
   /// dtor
   ~RenderEngine();

   /// sets scenegraph
   void SetScenegraph(std::shared_ptr<IScenegraph> spScenegraph);

   /// sets camera
   void SetCamera(std::shared_ptr<ICamera> spCamera);

   /// returns modifiable render options
   RenderOptions& GetRenderOptions();

   /// returns current camera
   std::shared_ptr<ICamera> GetCamera();

   /// returns render window
   RenderWindow& GetRenderWindow();

   /// renders scene
   void Render();

   /// swap display buffers
   void SwapBuffers();

private:
   /// applies render options to OpenGL
   void ApplyRenderOptions(RenderOptions& renderOptions);

private:
   class Impl;

   /// implementation
   std::shared_ptr<Impl> m_spImpl;
};
