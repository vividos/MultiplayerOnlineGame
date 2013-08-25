//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
/// \file ProgressBar.cpp Progress bar control
//

// includes
#include "StdAfx.h"
#include "ProgressBar.hpp"

void ProgressBar::Render(Rect& rectArea) throw()
{
   int iMid = int(rectArea.Left() + DividerPoint());

   glBegin(GL_QUADS);
      // render background quad
      glColor4ubv(m_backColor.m_color);
      glVertex2i(rectArea.Left(), rectArea.Top());
      glVertex2i(rectArea.Left(), rectArea.Bottom());
      glVertex2i(rectArea.Right(), rectArea.Bottom());
      glVertex2i(rectArea.Right(), rectArea.Top());

      // render foreground quad
      glColor4ubv(m_color.m_color);
      glVertex2i(rectArea.Left(), rectArea.Top());
      glVertex2i(rectArea.Left(), rectArea.Bottom());
      glVertex2i(iMid, rectArea.Bottom());
      glVertex2i(iMid, rectArea.Top());
   glEnd();
}

void ProgressBar::OnAttributeChanged(const CString& cszAttributeName) throw()
{
   if (ProgressBarAttr::Current == cszAttributeName)
   {
      m_uiPos = GetAttrAsUnsignedInt(cszAttributeName);
   }
   else
   if (ProgressBarAttr::Max == cszAttributeName)
   {
      m_uiMax = GetAttrAsUnsignedInt(cszAttributeName);
   }
   else
   if (ProgressBarAttr::Color == cszAttributeName)
   {
      m_color = GetAttrAsColor(cszAttributeName);
   }
   else
   if (ProgressBarAttr::BackColor == cszAttributeName)
   {
      m_backColor = GetAttrAsColor(cszAttributeName);
   }
}

unsigned int ProgressBar::DividerPoint() const
{
   double dPos = Clamp(0.0, 1.0, double(m_uiPos) / m_uiMax);

   Rect rectArea = GetRect();
   return static_cast<unsigned int>(rectArea.Width() * dPos);
}
