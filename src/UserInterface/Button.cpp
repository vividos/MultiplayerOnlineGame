//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Button.cpp Button control
//

// includes
#include "StdAfx.h"
#include "IWindowManager.hpp"
#include "Button.hpp"
#include "Font.hpp"
#include <SDL.h>
#include <functional>

void Button::Render(Rect& rectArea)
{
   Point pt1(rectArea.Left(), rectArea.Top());
   Point pt2(rectArea.Right(), rectArea.Bottom());

   int iMid = pt1.Y() + (pt2.Y()-pt1.Y()) / 2;

   glDisable(GL_TEXTURE_2D);

   // draw border, depending on "pressed" and "focused" state
   glLineWidth(2.0);

   glBegin(GL_LINES);
      glColor4ub(0,0,0, 255);

      // left line, down
      glVertex2i(pt1.X(), pt1.Y()+1);
      glVertex2i(pt1.X(), pt2.Y()-1);

         glVertex2i(pt1.X(), pt2.Y()-1);
         glVertex2i(pt1.X()+1, pt2.Y());

      if (m_bStatePressed)
      {
         glVertex2i(pt1.X()+1, pt1.Y()+1);
         glVertex2i(pt1.X()+1, pt2.Y()-1);
      }

      // bottom line, to the left
      glVertex2i(pt1.X()+1, pt2.Y());
      glVertex2i(pt2.X()-1, pt2.Y());

         glVertex2i(pt2.X()-1, pt2.Y());
         glVertex2i(pt2.X(), pt2.Y()-1);

      // right line, up
      glVertex2i(pt2.X(), pt2.Y()-1);
      glVertex2i(pt2.X(), pt1.Y()+1);

         glVertex2i(pt2.X(), pt1.Y()+1);
         glVertex2i(pt2.X()-1, pt1.Y());

      // top line, to the right
      glVertex2i(pt2.X()-1, pt1.Y());
      glVertex2i(pt1.X()+1, pt1.Y());

         glVertex2i(pt1.X()+1, pt1.Y());
         glVertex2i(pt1.X(), pt1.Y()+1);

      if (m_bStatePressed)
      {
         glVertex2i(pt2.X()-1, pt1.Y()+1);
         glVertex2i(pt1.X()+1, pt1.Y()+1);
      }

   glEnd();

   glLineWidth(1.0);

   if (m_bStatePressed)
      pt1.Add(1,1);

   pt1.Add(1,1);
   pt2.Add(-1,-1);

   // draw interior
   Color cTop(0xf3, 0xf3, 0xf3, 0xff);
   Color cTopLower(0xeb, 0xeb, 0xeb, 0xff);

   Color cBottom(0xcd, 0xcd, 0xcd, 0xff);
   Color cBottomHigher(0xdd, 0xdd, 0xdd, 0xff);

   glBegin(GL_QUADS);
      glColor4ubv(cTop.m_color);       glVertex2i(pt1.X(), pt1.Y());
      glColor4ubv(cTopLower.m_color);  glVertex2i(pt1.X(), iMid);
      glColor4ubv(cTopLower.m_color);  glVertex2i(pt2.X(), iMid);
      glColor4ubv(cTop.m_color);       glVertex2i(pt2.X(), pt1.Y());

      glColor4ubv(cBottomHigher.m_color); glVertex2i(pt1.X(), iMid);
      glColor4ubv(cBottom.m_color);       glVertex2i(pt1.X(), pt2.Y());
      glColor4ubv(cBottom.m_color);       glVertex2i(pt2.X(), pt2.Y());
      glColor4ubv(cBottomHigher.m_color); glVertex2i(pt2.X(), iMid);
   glEnd();

   // draw text
   if (!m_text.IsInit())
      UpdateTexture();

   if (m_bStatePressed)
      pt2.Add(1,1);

   Rect rectText(pt1, Size(pt2.X()-pt1.X(), pt2.Y()-pt1.Y()));
   m_text.Render(rectText);
}

bool Button::OnMouseButtonEvent(bool bPressed, int iMouseButton, unsigned int /*x*/, unsigned int /*y*/)
{
   if (iMouseButton == SDL_BUTTON_LEFT)
   {
      m_bStatePressed = bPressed;

      if (bPressed)
      {
         // start tracking if mouse leaves our area
         GetWindowManager().TrackMouseLeave(GetRect(),
            std::bind(&Button::OnMouseLeft, this));

         GetWindowManager().PlayAudioEvent(uiButtonPress);
      }

      if (!bPressed)
      {
         FireEvent(ButtonEvent::Clicked);

         GetWindowManager().PlayAudioEvent(uiButtonLeave);
      }
   }
   return true;
}

void Button::OnMouseMotionEvent(unsigned int /*x*/, unsigned int /*y*/)
{
   int x, y;
   Uint32 uiButtonState = ::SDL_GetMouseState(&x, &y);
   bool bLeftButtonDown = (uiButtonState & SDL_BUTTON_LMASK) != 0;

   // when mouse enters again...
   if (!m_bStatePressed && bLeftButtonDown)
   {
      // re-press button
      m_bStatePressed = true;
   }
}

bool Button::OnKeyboardEvent(bool bKeyDown, unsigned int sym, unsigned int mod)
{
   if (!bKeyDown && (sym == SDLK_RETURN || sym == SDLK_SPACE) && mod == 0)
      FireEvent(ButtonEvent::Clicked);
   return true;
}

void Button::OnAttributeChanged(const CString& cszAttributeName) throw()
{
   if (Parent() == NULL)
      return;

   if (cszAttributeName == ButtonAttr::Text)
      UpdateTexture();
}

void Button::OnMouseLeft()
{
   m_bStatePressed = false;
}

void Button::UpdateTexture()
{
   Font& font = GetFont();

   Size s = GetSize();
   CString cszText = GetAttr(ButtonAttr::Text);

   m_text.UpdateText(font, cszText, s, TextTexture::textAlignCenter,
      Color::Black(), Color::Transparent());
}
