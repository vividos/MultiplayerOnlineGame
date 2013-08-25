//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
/// \file EditField.cpp Edit field control
//

// includes
#include "StdAfx.h"
#include "EditField.hpp"
#include "Font.hpp"
#include "IWindowManager.hpp"
#include "IMouseEventReceiver.hpp"
#include <SDL.h>

void EditField::Render(Rect& rectArea) throw()
{
   if (!m_text.IsInit())
      UpdateTexture();

   m_text.Render(rectArea);

   // render border
   {
      glLineWidth(2.0f);

      //if (m_bShowBorder) // TODO
      {
         glBegin(GL_LINE_LOOP);
            Color m_cForeground = Color::Black(); // TODO
            glColor4ubv(m_cForeground.m_color);
            glVertex2i(rectArea.Left(),  rectArea.Top());
            glVertex2i(rectArea.Left(),  rectArea.Bottom());
            glVertex2i(rectArea.Right(), rectArea.Bottom());
            glVertex2i(rectArea.Right(), rectArea.Top());
         glEnd();
      }
   }

   // draw caret
   if (m_bFocused)
   {
      glBegin(GL_LINES);
         glColor4ubv(Color::Blue().m_color);
         glVertex2i(rectArea.Left() + m_uiCaretX, rectArea.Top() + 2);
         glVertex2i(rectArea.Left() + m_uiCaretX, rectArea.Top() + m_uiCaretHeight + 2);
      glEnd();
   }
}

bool EditField::OnMouseButtonEvent(bool bPressed, int iMouseButton, unsigned int /*x*/, unsigned int /*y*/)
{
   if (bPressed && iMouseButton == buttonLeft)
   {
      // set focus to ourselves
      GetWindowManager().SetFocus(shared_from_this());
      m_bFocused = true;
   }
   return true;
}

void EditField::OnMouseMotionEvent(unsigned int /*x*/, unsigned int /*y*/)
{
}

bool EditField::OnKeyboardEvent(bool bKeyDown, unsigned int sym, unsigned int mod)
{
   if (!bKeyDown)
      return true;

   switch (sym)
   {
   case SDLK_LEFT:
      if (m_uiCaretPos > 0)
         m_uiCaretPos--;
      UpdateCaretPos();
      break;

   case SDLK_RIGHT:
      if (m_uiCaretPos < static_cast<unsigned int>(m_cszText.GetLength()))
         m_uiCaretPos++;
      UpdateCaretPos();
      break;

   case SDLK_HOME:
      m_uiCaretPos = 0;
      UpdateCaretPos();
      break;

   case SDLK_END:
      m_uiCaretPos = m_cszText.GetLength();
      UpdateCaretPos();
      break;

   case SDLK_DELETE:
      if (m_uiCaretPos < static_cast<unsigned int>(m_cszText.GetLength()))
      {
         m_cszText.Delete(m_uiCaretPos);
         UpdateTexture();
      }
      break;

   case SDLK_BACKSPACE:
      if (m_uiCaretPos > 0)
      {
         m_cszText.Delete(--m_uiCaretPos);
         UpdateTexture();
      }
      break;

   case SDLK_ESCAPE:
      GetWindowManager().SetFocus();
      m_bFocused = false;
      break;

   default:
      if (IsAllowedChar(sym, mod))
      {
         unsigned int ch = sym;
         if (sym >= SDLK_a && sym <= SDLK_z && (mod & KMOD_SHIFT) != 0)
            ch -= 32; // make uppercase

         m_cszText.Insert(m_uiCaretPos++, static_cast<TCHAR>(ch));
         UpdateTexture();
      }
      break;
   }

   // update text attribute
   SetAttr(EditFieldAttr::Text, m_cszText, true);

   return true;
}

void EditField::OnFocusChanged(bool bGotFocus)
{
   m_bFocused = bGotFocus;
   UpdateTexture();
}

bool EditField::IsAllowedChar(unsigned int sym, unsigned int /*mod*/)
{
   // only allow printable ascii chars
   if (sym >= 32 && sym < 128)
      return true;
   return false;
}

void EditField::OnAttributeChanged(const CString& cszAttributeName) throw()
{
   bool bUpdate = false;

   if (EditFieldAttr::Text == cszAttributeName)
   {
      m_cszText = GetAttr(cszAttributeName);
      m_uiCaretPos = m_cszText.GetLength();
      bUpdate = true;
   }
   else
   if (EditFieldAttr::Password == cszAttributeName)
      bUpdate = true;

   if (bUpdate && Parent() != NULL)
      UpdateTexture();
}

CString EditField::GetDisplayText() const
{
   bool bPassword = GetAttrAsBool(EditFieldAttr::Password);

   if (bPassword)
      return CString(_T('*'), m_cszText.GetLength());

   return m_cszText;
}

void EditField::UpdateTexture()
{
   Font& font = GetFont();

   Size s = GetSize();

   m_text.UpdateText(font, GetDisplayText(), s, TextTexture::textAlignLeft,
      Color::Black(), Color::White()); // TODO settable colors

   UpdateCaretPos();
}

void EditField::UpdateCaretPos()
{
   // calculate position of caret
   Font& font = GetFont();
   m_uiCaretX = font.MeasureString(CString(GetDisplayText(), m_uiCaretPos));
   m_uiCaretHeight = font.Height();
}
