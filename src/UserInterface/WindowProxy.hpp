//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
/// \file WindowProxy.hpp Proxy class for Window member variables
//
#pragma once

// includes
#include "Window.hpp"

/// \brief proxy class for a window
/// \details used for Window instances that are member variables of a class, but
/// a shared_ptr to the window is needed.
class WindowProxy: public Window
{
public:
   /// ctor
   WindowProxy(Window& wnd) throw()
      :m_wnd(wnd)
   {
   }

   /// dtor
   virtual ~WindowProxy()
   {
      m_wnd.m_spParent.reset();
   }

   virtual Window& RealWindow() throw() override { return m_wnd; }

   virtual const Window& RealWindow() const throw() override { return m_wnd; }

   virtual void RegisterChild(WindowPtr spWnd) override
   {
      m_wnd.RegisterChild(spWnd);
   }
   virtual void UnregisterChild(WindowPtr spWnd) override { m_wnd.UnregisterChild(spWnd); }

   virtual void Render(Rect& rectArea) override
   {
      m_wnd.m_spParent = Parent();
      m_wnd.Render(rectArea);
   }

   virtual bool OnMouseButtonEvent(bool bPressed, int iMouseButton, unsigned int x, unsigned int y) override
   {
      return m_wnd.OnMouseButtonEvent(bPressed, iMouseButton, x, y);
   }

   virtual void OnMouseMotionEvent(unsigned int x, unsigned int y) override
   {
      m_wnd.OnMouseMotionEvent(x, y);
   }

   virtual bool OnKeyboardEvent(bool bKeyDown, unsigned int sym, unsigned int mod) override
   {
      return m_wnd.OnKeyboardEvent(bKeyDown, sym, mod);
   }

private:
   /// actual window
   Window& m_wnd;
};
