//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Panel.cpp Panel window
//

// includes
#include "StdAfx.h"
#include "Panel.hpp"
#include "IWindowManager.hpp"

void Panel::Render(Rect& rectArea) throw()
{
   if (GetAttrAsBool(PanelAttr::ShowBackground))
   {
      // first render background of panel
      glDisable(GL_TEXTURE_2D);

      Color cBackground = GetAttrAsColor(PanelAttr::BackgroundColor);

      glBegin(GL_QUADS);
         glColor4f(cBackground.m_color[0]/255.0f,
            cBackground.m_color[1]/255.0f,
            cBackground.m_color[2]/255.0f,
            cBackground.m_color[3]/255.0f);

         glVertex2i(rectArea.Left(), rectArea.Top());
         glVertex2i(rectArea.Left(), rectArea.Bottom());
         glVertex2i(rectArea.Right(), rectArea.Bottom());
         glVertex2i(rectArea.Right(), rectArea.Top());

         glColor4f(1.0, 1.0, 1.0, 1.0);
      glEnd();

      glEnable(GL_TEXTURE_2D);
   }

   // calculate panel's area
   Rect rectPanel(GetPos(), GetSize());

   // then render all child windows
   for (size_t i=0, iMax = m_vecAllChildWindows.size(); i<iMax; i++)
   {
      WindowPtr spWnd = m_vecAllChildWindows[i];
      Window& wnd = spWnd->RealWindow();

      Rect rcChild = wnd.GetRect();
      rcChild.Add(Point(rectPanel.Left(), rectPanel.Top()));

      spWnd->Render(rcChild);
   }
}

bool Panel::OnMouseButtonEvent(bool bPressed, int iMouseButton, unsigned int x, unsigned int y)
{
   bool bHandled = false;
   Point pt(x,y);
   for (size_t i=0, iMax = m_vecAllChildWindows.size(); i<iMax; i++)
   {
      WindowPtr spWnd = m_vecAllChildWindows[i];
      Window& wnd = spWnd->RealWindow(); // get window, in case it's a WindowProxy instance

      Rect rc = wnd.GetRect();
      rc.Add(GetPos()); // add parent's position

      if (rc.IsInside(pt) || GetWindowManager().IsTrackedMouseUp(spWnd, iMouseButton))
      {
         bHandled = wnd.OnMouseButtonEvent(bPressed, iMouseButton, x, y);
         break;
      }
   }

   if (bHandled)
      return true;

   // only signal "handled" state when panel background is shown
   return GetAttrAsBool(PanelAttr::ShowBackground) ? true : bHandled;
}

void Panel::OnMouseMotionEvent(unsigned int x, unsigned int y)
{
   Point pt(x,y);
   for (size_t i=0, iMax = m_vecAllChildWindows.size(); i<iMax; i++)
   {
      WindowPtr spWnd = m_vecAllChildWindows[i];
      Window& wnd = spWnd->RealWindow(); // get window, in case it's a WindowProxy instance

      Rect rc = wnd.GetRect();
      rc.Add(GetPos()); // add parent's position

      if (rc.IsInside(pt))
         wnd.OnMouseMotionEvent(x, y);
   }
}

WindowPtr Panel::FindByName(LPCTSTR pszWindowName)
{
   // search all windows, beginning with the root window
   for (size_t i=0, iMax=m_vecAllChildWindows.size(); i<iMax; i++)
   {
      WindowPtr spWindow = m_vecAllChildWindows[i];

      if (spWindow->GetAttr(WindowAttr::Name) == pszWindowName)
         return spWindow;

      // search sub windows, if any
      std::shared_ptr<Panel> spPanel = std::dynamic_pointer_cast<Panel, Window>(spWindow);
      if (spPanel != NULL)
      {
         WindowPtr spSubWindow = spPanel->FindByName(pszWindowName);
         if (spSubWindow != NULL)
            return spSubWindow; // found
      }
   }

   return WindowPtr();
}
