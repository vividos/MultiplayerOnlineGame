//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IWindowManager.hpp Window manager interface
//
#pragma once

// includes
#include <boost/noncopyable.hpp>
#include <functional>
#include "UserInterfaceAudio.hpp"

// forward references
class Size;
class Window;
class Rect;

/// Window smart pointer
typedef std::shared_ptr<Window> WindowPtr;

/// window manager interface
class IWindowManager: public boost::noncopyable
{
public:
   DEFINE_INSTANCE(IWindowManager)

   /// dtor
   virtual ~IWindowManager() throw()
   {
   }

   /// sets audio event handler
   virtual void SetAudioEventHandler(T_fnOnUserInterfaceAudioEvent fnOnUserInterfaceAudioEvent) throw() = 0;

   /// called before rendering user interface
   virtual void PreRender() = 0;
   /// called to render user interface
   virtual void Render() = 0;

   /// called to handle mouse button events
   virtual bool OnMouseButtonEvent(bool bPressed, int iMouseButton, unsigned int x, unsigned int y) = 0;
   /// called to handle mouse motion events
   virtual void OnMouseMotionEvent(unsigned int x, unsigned int y) = 0;
   /// called to handle keyboard events
   virtual bool OnKeyboardEvent(bool bKeyDown, unsigned int sym, unsigned int mod) = 0;
   /// called when screen has been resized (e.g. toggling fullscreen)
   virtual void OnResizeScreen(Size newSize) = 0;

   /// returns root window ptr
   virtual WindowPtr GetRootWindow() throw() = 0;

   /// finds window by name; returns nullptr when not found
   virtual WindowPtr FindWindow(LPCTSTR pszWindowName) throw() = 0;

   /// finds window by name, and casts it to control class
   template <typename TControl>
   std::shared_ptr<TControl> FindWindow(LPCTSTR pszWindowName)
   {
      WindowPtr spWindow = FindWindow(pszWindowName);
      return std::dynamic_pointer_cast<TControl>(spWindow);
   }

   /// sets focus to given window; all other windows in the same panel lose focus
   virtual void SetFocus(WindowPtr spFocusedWnd = WindowPtr()) throw() = 0;

   /// starts tracking mouse and calls function when leaving rect
   virtual void TrackMouseLeave(const Rect& rect, std::function<void()> fnCallback) throw() = 0;

   /// plays back audio event
   virtual void PlayAudioEvent(T_enUserInterfaceAudioEvents enUserInterfaceAudioEvent) throw() = 0;
};
