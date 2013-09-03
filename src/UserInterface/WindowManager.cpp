//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
/// \file WindowManager.cpp Window manager implementation
//

// includes
#include "StdAfx.h"
#include "WindowManager.hpp"
#include "Panel.hpp"
#include "JsonWindowLoader.hpp"
#include <ulib/stream/TextStreamFilter.hpp>
#include "Path.hpp"
#include "IFileSystem.hpp"
#include <SDL.h>

// auto-link
#pragma comment(lib, "glu32.lib") // for gluOrtho2D

unsigned int Observable::s_uiNextEventId = 0;

//
// RootWindow
//

/// root window for all child windows in the window manager
class RootWindow: public Panel
{
public:
   /// ctor
   RootWindow(IWindowManager& windowManager, FontManager& fontManager)
      :m_windowManager(windowManager),
       m_spFont(fontManager.Create(FontTypeface::Normal, 16))
   {
   }

   /// dtor
   virtual ~RootWindow() throw() {}

   /// returns window manager
   virtual IWindowManager& GetWindowManager() override
   {
      return m_windowManager;
   }

   /// returns font
   virtual Font& GetFont() override
   {
      return *m_spFont;
   }

private:
   /// window manager
   IWindowManager& m_windowManager;

   /// font
   FontPtr m_spFont;
};


//
// WindowManager
//

WindowManager::WindowManager()
{
   m_spRootWindow.reset(new RootWindow(*this, m_fontManager));

   // create root window from surface size
   SDL_Surface* surf = SDL_GetVideoSurface();
   ATLASSERT(surf != NULL); // window not opened?

   Window& w = *m_spRootWindow;

   // set attributes
   w[WindowAttr::Pos] = _T("0,0");
   w.SetAttr(WindowAttr::Size, Size(surf->w, surf->h));
}

CString WindowManager::LoadJson(IFileSystem& fileSystem, const CString& cszRelativeFilename)
{
   std::shared_ptr<Stream::IStream> spStream =
      fileSystem.OpenFile(cszRelativeFilename, true);

   Stream::TextStreamFilter stream(*spStream,
      Stream::ITextStream::textEncodingAnsi,
      Stream::ITextStream::lineEndingNative);

   CString cszLine, cszText;
   while (!stream.AtEndOfStream())
   {
      stream.ReadLine(cszLine);
      cszText += cszLine;
      cszText += _T("\r\n");
   }

   return cszText;
}

void WindowManager::CreateFromJson(WindowPtr spWindow, IFileSystem& fileSystem, const CString& cszRelativeFilename)
{
   CString cszJsonText = LoadJson(fileSystem, cszRelativeFilename);

   JsonWindowLoader loader;
   loader.Load(spWindow, cszJsonText);
}

void WindowManager::MapCoords(unsigned int& x, unsigned int& y)
{
   // get surface rect
   SDL_Surface* surf = SDL_GetVideoSurface();

   Size s = m_spRootWindow->GetSize();

   // translate to window coords
   x = static_cast<unsigned int>((static_cast<double>(s.Width())/surf->w) * x);
   y = static_cast<unsigned int>((static_cast<double>(s.Height())/surf->h) * y);
}

void WindowManager::PreRender()
{
}

void WindowManager::Render()
{
   // setup orthogonal projection
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   Size s = m_spRootWindow->GetSize();
   gluOrtho2D(0, s.Width(), s.Height(), 0);

   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();

   // set states
   glDisable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_LIGHTING);

   // set up texturing
   glEnable(GL_TEXTURE_2D);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   glColor3ub(255, 255, 255);

   // render root window; all child windows are rendered recursively
   Rect rectArea(Point(0,0), m_spRootWindow->GetSize());
   m_spRootWindow->Render(rectArea);

   // restore previous projection
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();

   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();

   glDisable(GL_BLEND);
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_TEXTURE_2D);
}

bool WindowManager::OnMouseButtonEvent(bool bPressed, int iMouseButton, unsigned int x, unsigned int y)
{
   MapCoords(x, y);

   // hand over to root window
   return m_spRootWindow->OnMouseButtonEvent(bPressed, iMouseButton, x, y);
}

void WindowManager::OnMouseMotionEvent(unsigned int x, unsigned int y)
{
   MapCoords(x, y);

   // hand over to root window
   m_spRootWindow->OnMouseMotionEvent(x, y);

   // check if mouse left any rectangle
   if (!m_listAllTrackMouseLeaveEntries.empty())
   {
      std::list<T_TrackMouseLeaveEntry>::iterator iter = m_listAllTrackMouseLeaveEntries.begin(),
         stop = m_listAllTrackMouseLeaveEntries.end();

      Point p(x,y);
      while (iter != stop)
      {
         const T_TrackMouseLeaveEntry& entry = *iter;
         if (!entry.first.IsInside(p))
         {
            entry.second();

            // remove entry; returns iterator to next element
            iter = m_listAllTrackMouseLeaveEntries.erase(iter);
         }
         else
            ++iter;
      }
   }
}

bool WindowManager::OnKeyboardEvent(bool bKeyDown, unsigned int sym, unsigned int mod)
{
   // hand over to focused window, if any
   if (m_spFocusedWindow != NULL)
      return m_spFocusedWindow->OnKeyboardEvent(bKeyDown, sym, mod);

   return false;
}

WindowPtr WindowManager::GetRootWindow() throw()
{
   ATLASSERT(m_spRootWindow != NULL);
   return m_spRootWindow;
}

WindowPtr WindowManager::FindWindow(LPCTSTR pszWindowName) throw()
{
   // check root window
   WindowPtr spWindow = m_spRootWindow;

   if (spWindow->GetAttr(WindowAttr::Name) == pszWindowName)
      return spWindow;

   // since the root window is a Panel, ask it
   std::shared_ptr<Panel> spPanel = std::dynamic_pointer_cast<Panel, Window>(spWindow);
   ATLASSERT(spPanel != NULL);

   return spPanel->FindByName(pszWindowName);
}

void WindowManager::SetFocus(WindowPtr spFocusedWnd) throw()
{
   if (m_spFocusedWindow != NULL)
      m_spFocusedWindow->OnFocusChanged(false);

   m_spFocusedWindow = spFocusedWnd;

   if (m_spFocusedWindow != NULL)
      m_spFocusedWindow->OnFocusChanged(true);
}

void WindowManager::TrackMouseLeave(const Rect& rect, std::function<void()> fnCallback) throw()
{
   m_listAllTrackMouseLeaveEntries.push_back(std::make_pair(rect, fnCallback));
}

void WindowManager::PlayAudioEvent(T_enUserInterfaceAudioEvents enUserInterfaceAudioEvent) throw()
{
   if (m_fnOnUserInterfaceAudioEvent != NULL)
   {
      try
      {
         m_fnOnUserInterfaceAudioEvent(enUserInterfaceAudioEvent);
      }
      catch(...)
      {
      }
   }
}
