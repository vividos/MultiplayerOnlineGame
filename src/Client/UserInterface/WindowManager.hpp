//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file WindowManager.hpp Window manager implementation
//
#pragma once

// includes
#include "UserInterfaceCommon.hpp"
#include "Window.hpp"
#include "IWindowManager.hpp"
#include "FontManager.hpp"
#include <functional>

// forward references
class IFileSystem;

/// window manager
class USERINTERFACE_DECLSPEC WindowManager: public IWindowManager
{
public:
   /// ctor
   WindowManager(unsigned int uiWidth, unsigned int uiHeight);

   /// dtor
   virtual ~WindowManager()
   {
   }

   /// loads window content by (base) filename
   static void CreateFromJson(WindowPtr spWindow, IFileSystem& fileSystem, const CString& cszRelativeFilename);

private:
   /// loads json from relative filename
   static CString LoadJson(IFileSystem& fileSystem, const CString& cszRelativeFilename);

   /// maps coordinates to window coordinates
   void MapCoords(unsigned int& x, unsigned int& y);

   /// clear "mouse up" tracked windows
   void ClearTrackMouseUpWindows(int iMouseButton);

   // methods from IWindowManager

   /// sets audio event handler
   virtual void SetAudioEventHandler(T_fnOnUserInterfaceAudioEvent fnOnUserInterfaceAudioEvent) override
   {
      m_fnOnUserInterfaceAudioEvent = fnOnUserInterfaceAudioEvent;
   }

   /// called before rendering user interface
   virtual void PreRender() override;
   /// called to render user interface
   virtual void Render() override;

   /// called to handle mouse button events
   virtual bool OnMouseButtonEvent(bool bPressed, int iMouseButton, unsigned int x, unsigned int y) override;
   /// called to handle mouse motion events
   virtual void OnMouseMotionEvent(unsigned int x, unsigned int y) override;
   /// called to handle keyboard events
   virtual bool OnKeyboardEvent(bool bKeyDown, unsigned int sym, unsigned int mod) override;
   /// called when screen has been resized (e.g. toggling fullscreen)
   virtual void OnResizeScreen(Size newSize) override;

   /// returns root window
   virtual WindowPtr GetRootWindow() override;

   /// finds window by name; returns nullptr when not found
   virtual WindowPtr FindWindow(LPCTSTR pszWindowName) override;

   /// sets (or resets) focus to a specific window
   virtual void SetFocus(WindowPtr spFocusedWnd = WindowPtr()) override;

   /// starts tracking mouse and calls function when leaving rect
   virtual void TrackMouseLeave(const Rect& rect, std::function<void()> fnCallback) override;

   /// starts tracking mouse until mouse button is up, regardless if mouse leaves window
   virtual void TrackMouseUp(WindowPtr spWindow, int iMouseButton) override;

   /// returns if given window is tracked by call to TrackMouseUp()
   virtual bool IsTrackedMouseUp(WindowPtr spWindow, int iMouseButton) override;

   /// plays back audio event
   virtual void PlayAudioEvent(T_enUserInterfaceAudioEvents enUserInterfaceAudioEvent) override;

private:
   /// current screen size
   Size m_screenSize;

   /// root window for all child windows
   WindowPtr m_spRootWindow;

   /// font manager
   FontManager m_fontManager;

   /// currently focused window
   WindowPtr m_spFocusedWindow;

   /// entry from TrackMouseLeave() function
   typedef std::pair<Rect, std::function<void()> > T_TrackMouseLeaveEntry;

   /// all entries for tracking mouse leave events
   std::list<T_TrackMouseLeaveEntry> m_listAllTrackMouseLeaveEntries;

   /// type for mapping from mouse button to set of windows to track
   typedef std::map<int, std::set<WindowPtr>> T_MapTrackMouseUpWindows;

   /// mapping from mouse button to set of windows to track until mouse up
   T_MapTrackMouseUpWindows m_mapTrackMouseUpWindows;

   /// callback for playing user interface audio events
   T_fnOnUserInterfaceAudioEvent m_fnOnUserInterfaceAudioEvent;
};
