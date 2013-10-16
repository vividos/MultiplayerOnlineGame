//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file RenderWindow.hpp Render window class
//
#pragma once

// includes
#include "RenderEngineCommon.hpp"
#include "RenderEngineCommon.hpp"

// forward references
class RenderOptions;
typedef struct SDL_Window SDL_Window;

/// \brief render window
class RENDERENGINE_DECLSPEC RenderWindow: public boost::noncopyable
{
public:
   /// ctor
   RenderWindow(const CString& cszWindowCaption, unsigned int uiWidth, unsigned int uiHeight, bool bFullscreen);

   /// swaps display buffers
   void SwapBuffers();

   /// resizes view
   void ResizeView(unsigned int uiWidth, unsigned int uiHeight);

   /// change fullscreen or window mode
   void SetFullscreen(bool bFullscreen);

   /// sets new caption
   void SetCaption(const CString& cszCaption);

   /// sets new mouse pos; causes a SDL_MOUSEMOVE event
   void SetMousePos(int xpos, int ypos);

   /// returns window size
   void GetWindowSize(int& iWidth, int& iHeight) const;

private:
   /// initializes video
   void InitVideo(const CString& cszWindowCaption, unsigned int uiWidth, unsigned int uiHeight, bool bFullscreen);

private:
   /// SDL window
   std::shared_ptr<SDL_Window> m_spWindow;

   /// GL context
   std::shared_ptr<void> m_spGLCcontext;
};
