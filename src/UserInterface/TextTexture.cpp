//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TextTexture.cpp Texture that contains text
//

// includes
#include "StdAfx.h"
#include "TextTexture.hpp"
#include "Font.hpp"
#include "Bitmap.hpp"

void TextTexture::Render(Rect rc)
{
   // draw text
   glEnable(GL_TEXTURE_2D);
   m_tex.Bind();

   double u = 1.0;
   double v = 1.0;

   glBegin(GL_QUADS);
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      glTexCoord2d(0.0, 0.0); glVertex2i(rc.Left(),  rc.Top());
      glTexCoord2d(0.0,   v); glVertex2i(rc.Left(),  rc.Bottom());
      glTexCoord2d(  u,   v); glVertex2i(rc.Right(), rc.Bottom());
      glTexCoord2d(  u, 0.0); glVertex2i(rc.Right(), rc.Top());
   glEnd();

   glDisable(GL_TEXTURE_2D);
}

void TextTexture::UpdateText(Font& font,
   const CString& cszText, const Size& s, T_enTextAlignment enTextAlignment,
   Color cForeground, Color cBackground)
{
   unsigned int uiLenPx = font.MeasureString(cszText);
   unsigned int uiPosX = 0;

   if (enTextAlignment == textAlignCenter)
      uiPosX = uiLenPx < unsigned(s.Width()) ? (unsigned(s.Width()) - uiLenPx) / 2 : 0;

   unsigned int uiPosY = font.Height() < unsigned(s.Height()) ? (unsigned(s.Height()) - font.Height()) / 2 : 0;

   Bitmap bmp(s.Width(), s.Height(), cBackground);
   font.Write(cszText, bmp, uiPosX, uiPosY, cForeground, cBackground);

   if (m_tex.Id() == 0)
      m_tex.Generate();

   m_tex.Upload(bmp, false);
}
