//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Checkbox.cpp Checkbox control
//

// includes
#include "stdafx.h"
#include "Checkbox.hpp"
#include <SDL.h>

void Checkbox::Render(Rect& rectArea)
{
   RenderCheck(rectArea);

   if (!m_text.IsInit())
      UpdateTexture();

   m_text.Render(rectArea);
}

void Checkbox::RenderCheck(Rect& rectArea)
{
   Rect rectBox = rectArea;

   int iBoxMargin = int(rectArea.Height() * 0.2);
   int iBoxMarginRight = int(rectArea.Height() * 0.8 * 0.75);

   rectBox.Left(rectBox.Left() + iBoxMargin);
   rectBox.Right(rectBox.Left() + iBoxMarginRight);
   rectBox.Top(rectBox.Top() + iBoxMargin);
   rectBox.Bottom(rectBox.Bottom() - iBoxMargin);

   OpenGL::PushedAttributes attr(GL_ENABLE_BIT | GL_LINE_BIT | GL_POINT_BIT);

   glDisable(GL_TEXTURE_2D);
   glEnable(GL_LINE_SMOOTH);
   glEnable(GL_POINT_SMOOTH);

   Color cBox = GetAttrAsColor(CheckboxAttr::BoxColor);
   glColor4ubv(cBox.m_color);
   glLineWidth(2.0);
   glBegin(GL_LINE_LOOP);
      glVertex2i(rectBox.Left(), rectBox.Bottom());
      glVertex2i(rectBox.Right(), rectBox.Bottom());
      glVertex2i(rectBox.Right(), rectBox.Top());
      glVertex2i(rectBox.Left(), rectBox.Top());
   glEnd();

   if (IsChecked())
   {
      double iCheckUnit = rectBox.Height() * 0.1;

      Point point1(int(rectBox.Left() + 3 * iCheckUnit), int(rectBox.Bottom() - 6 * iCheckUnit));
      Point point2(int(rectBox.Left() + 6 * iCheckUnit), int(rectBox.Bottom() - 3 * iCheckUnit));
      Point point3(int(rectBox.Left() + 11* iCheckUnit), int(rectBox.Bottom() - 11* iCheckUnit));

      Color cCheck = GetAttrAsColor(CheckboxAttr::CheckColor);
      glColor4ubv(cCheck.m_color);
      glLineWidth(4.0);
      glBegin(GL_LINES);
         glVertex2i(point1.X(), point1.Y());
         glVertex2i(point2.X(), point2.Y());
         glVertex2i(point2.X(), point2.Y());
         glVertex2i(point3.X(), point3.Y());
      glEnd();

      glPointSize(3.0);
      glBegin(GL_POINTS);
         glVertex2i(point1.X(), point1.Y());
         glVertex2i(point2.X(), point2.Y());
         glVertex2i(point3.X(), point3.Y());
      glEnd();
   }

   // adjust rect for size of checkbox
   int iOffset = int(rectArea.Height() * 1.1);
   rectArea.Left(rectArea.Left() + iOffset);
}

bool Checkbox::OnMouseButtonEvent(bool bPressed, int /*iMouseButton*/, unsigned int /*x*/, unsigned int /*y*/)
{
   if (bPressed)
      return false;

   SetCheck(!IsChecked());

   return true;
}

bool Checkbox::OnKeyboardEvent(bool bKeyDown, unsigned int sym, unsigned int /*mod*/)
{
   if (!bKeyDown && sym == SDLK_SPACE)
   {
      SetCheck(!IsChecked());
      return true;
   }

   return false;
}

void Checkbox::OnAttributeChanged(const CString& cszAttributeName) throw()
{
   if (Parent() == nullptr)
      return;

   if (cszAttributeName == CheckboxAttr::Text ||
      cszAttributeName == CheckboxAttr::BackColor ||
      cszAttributeName == CheckboxAttr::Color)
      UpdateTexture();
}

void Checkbox::UpdateTexture()
{
   Font& font = GetFont();

   Size s = GetSize();
   int iOffset = int(s.Height() * 1.1);
   s.Width(s.Width() - iOffset);

   CString cszText = GetAttr(CheckboxAttr::Text);
   Color cText = GetAttrAsColor(CheckboxAttr::Color);
   Color cBack = GetAttrAsColor(CheckboxAttr::BackColor);

   m_text.UpdateText(font, cszText, s, TextTexture::textAlignLeft, cText, cBack);
}
