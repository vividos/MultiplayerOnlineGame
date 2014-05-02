//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file RenderView.hpp Render view
//
#pragma once

// includes
#include <SDL.h>
#include "RenderEngine.hpp"
#include "RenderWindow.hpp"

/// RenderView win traits
typedef CWinTraits<WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS> RenderViewWinTraits;

/// render window
class RenderView :
   public CWindowImpl<RenderView, ATL::CWindow, RenderViewWinTraits>
{
   /// type of this class
   typedef CWindowImpl<RenderView, ATL::CWindow, RenderViewWinTraits> ThisClass;

public:
   /// ctor
   RenderView()
   {
      SDL_SetMainReady();
      SDL_Init(SDL_INIT_VIDEO);
   }

   /// dtor
   ~RenderView() throw()
   {
      SDL_QuitSubSystem(SDL_INIT_VIDEO);
   }

   /// translates messages
   BOOL PreTranslateMessage(MSG* /*pMsg*/)
   {
      return FALSE;
   }

   /// returns render engine
   std::shared_ptr<RenderEngine> GetRenderEngine() { return m_spRenderEngine; }

private:
   BEGIN_MSG_MAP(ThisClass)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      MESSAGE_HANDLER(WM_PAINT, OnPaint)
      MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
   END_MSG_MAP()

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      CClientDC dc(m_hWnd);

      // Fill in the pixel format descriptor.
      PIXELFORMATDESCRIPTOR pfd;
      memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
      pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
      pfd.nVersion   = 1;
      pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
      pfd.iPixelType = PFD_TYPE_RGBA;
      pfd.cColorBits = 24;
      pfd.cDepthBits = 32;
      pfd.iLayerType = PFD_MAIN_PLANE;

      int nPixelFormat = dc.ChoosePixelFormat(&pfd);
      ATLASSERT(nPixelFormat != 0);

      BOOL bResult = dc.SetPixelFormat(nPixelFormat, &pfd);
      ATLASSERT(bResult); bResult;

      m_hRC = dc.wglCreateContext();
      ATLASSERT(m_hRC);

      // create SDL window
      void* pData = m_hWnd;
      SDL_Window* pWindow = SDL_CreateWindowFrom(pData);

      m_spWindow = std::shared_ptr<SDL_Window>(pWindow, SDL_DestroyWindow);

      m_spRenderEngine.reset(new RenderEngine(m_spWindow));

      bHandled = FALSE;
      return 0;
   }

   LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      if (m_hRC)
      {
         ::wglDeleteContext(m_hRC);
         m_hRC = NULL;
      }

      m_spRenderEngine.reset();

      m_spWindow.reset();

      bHandled = FALSE;
      return 0;
   }

   LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      int cx = LOWORD(lParam);
      int cy = HIWORD(lParam);

      if (cx==0 || cy==0 || m_hWnd==NULL)
         return 0;

      CClientDC dc(m_hWnd);
      dc.wglMakeCurrent(m_hRC);

      m_spRenderEngine->GetRenderWindow().ResizeView(unsigned(cx), unsigned(cy));

      dc.wglMakeCurrent(NULL);

      return 0;
   }

   LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      CPaintDC dc(m_hWnd);
      dc.wglMakeCurrent(m_hRC);

      m_spRenderEngine->Render();

      dc.SwapBuffers();
      dc.wglMakeCurrent(nullptr);

      return 0;
   }

   LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
   {
      return 1;
   }

private:
   /// render engine
   std::shared_ptr<RenderEngine> m_spRenderEngine;

   /// Render context
   HGLRC m_hRC;

   /// SDL window
   std::shared_ptr<SDL_Window> m_spWindow;
};
