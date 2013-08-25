//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file RenderWindow.hpp Render window class
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"

// forward references
class RenderOptions;

/// \brief render window
class RENDERENGINE_DECLSPEC RenderWindow: public boost::noncopyable
{
public:
   /// ctor
   RenderWindow(unsigned int uiWidth, unsigned int uiHeight, bool bFullscreen, RenderOptions& renderOptions);

   /// sets up view
   void SetupView(RenderOptions& renderOptions);

   /// swaps display buffers
   void SwapBuffers();

   /// change fullscreen or window mode
   void SetFullscreen(bool bFullscreen);

private:
   /// initializes video
   void InitVideo(unsigned int uiWidth, unsigned int uiHeight, bool bFullscreen);

   /// resizes view
   void ResizeView(unsigned int uiWidth, unsigned int uiHeight);
};
