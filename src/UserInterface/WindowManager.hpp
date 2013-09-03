//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
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
   WindowManager();

   /// dtor
   virtual ~WindowManager() throw()
   {
   }

   /// loads window content by (base) filename
   static void CreateFromJson(WindowPtr spWindow, IFileSystem& fileSystem, const CString& cszRelativeFilename);

private:
   /// loads json from relative filename
   static CString LoadJson(IFileSystem& fileSystem, const CString& cszRelativeFilename);

   /// maps coordinates to window coordinates
   void MapCoords(unsigned int& x, unsigned int& y);

   // methods from IWindowManager

   /// sets audio event handler
   virtual void SetAudioEventHandler(T_fnOnUserInterfaceAudioEvent fnOnUserInterfaceAudioEvent) throw() override
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

   /// returns root window
   virtual WindowPtr GetRootWindow() throw() override;

   /// finds window by name; returns nullptr when not found
   virtual WindowPtr FindWindow(LPCTSTR pszWindowName) throw() override;

   /// sets (or resets) focus to a specific window
   virtual void SetFocus(WindowPtr spFocusedWnd = WindowPtr()) throw() override;

   /// starts tracking mouse and calls function when leaving rect
   virtual void TrackMouseLeave(const Rect& rect, std::function<void()> fnCallback) throw() override;

   /// plays back audio event
   virtual void PlayAudioEvent(T_enUserInterfaceAudioEvents enUserInterfaceAudioEvent) throw() override;

private:
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

   /// callback for playing user interface audio events
   T_fnOnUserInterfaceAudioEvent m_fnOnUserInterfaceAudioEvent;
};
