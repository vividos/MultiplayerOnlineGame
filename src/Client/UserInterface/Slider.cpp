//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Slider.cpp Slider control
//

// includes
#include "StdAfx.h"
#include "Slider.hpp"
#include "IWindowManager.hpp"
#include <SDL.h>

/// percent of height that consists of the bar
const double c_dBarHeightPercent = 0.3;

/// width of slider
const unsigned int c_uiSliderWidth = 25;

/// square root of 2, from Wolfram Alpha
const double c_dSquareRoot2 = 1.4142135623730950488016887242096980785696718753769480;

void Slider::Render(Rect& rectArea) throw()
{
   RenderProgressBar(rectArea);
   RenderSlider(rectArea);
}

void Slider::RenderProgressBar(Rect& rectArea) throw()
{
   Rect rectBar(rectArea);
   rectBar.Top(rectBar.Bottom() - int(rectBar.Height() * c_dBarHeightPercent));

   BaseClass::Render(rectBar);
}

void Slider::RenderSlider(Rect& rectArea) throw()
{
   int iMid = rectArea.Left() + int(DividerPoint());

   double dAspect = 1.33333;

   unsigned int uiRemainingHeight = unsigned(rectArea.Height() * (1.0 - c_dBarHeightPercent));
   unsigned int y = static_cast<unsigned int>((c_uiSliderWidth * dAspect / 2.0) / c_dSquareRoot2);
   unsigned int x = (uiRemainingHeight < y) ? 0 : (uiRemainingHeight - y);

   for (int i = 0; i<2; i++)
   {
      glBegin(i == 0 ? GL_POLYGON : GL_LINE_LOOP);

      if (i == 0)
         glColor4ubv(m_sliderColor.m_color);
      else
         glColor3ub(0, 0, 0);

      glVertex2i(iMid - c_uiSliderWidth / 2, rectArea.Top());
      glVertex2i(iMid - c_uiSliderWidth / 2, rectArea.Top() + x);
      glVertex2i(iMid, rectArea.Top() + uiRemainingHeight);
      glVertex2i(iMid + c_uiSliderWidth / 2, rectArea.Top() + x);
      glVertex2i(iMid + c_uiSliderWidth / 2, rectArea.Top());
      glEnd();
   }
}

bool Slider::OnMouseButtonEvent(bool bPressed, int iMouseButton, unsigned int x, unsigned int y)
{
   if (bPressed && IsInsideSliderArea(Point(x, y)))
   {
      // set focus to ourselves
      GetWindowManager().SetFocus(shared_from_this());
      m_bFocused = true;

      // start sliding
      m_bMouseButtonDown = true;

      // calc drag x offset
      Rect rect = Window::GetRect();
      unsigned int uiMid = DividerPoint();

      m_iDragOffset = x - int(rect.Left() + uiMid);

      // start mouse motion tracking until mouse button is up
      GetWindowManager().TrackMouseUp(shared_from_this(), iMouseButton);
   }
   else
      m_bMouseButtonDown = false;

   return true;
}

void Slider::OnMouseMotionEvent(unsigned int x, unsigned int /*y*/)
{
   if (!m_bMouseButtonDown)
      return;

   // check if slider pos has moved since
   unsigned int uiNewSliderPos = SliderPosByOffset(x - m_iDragOffset);

   if (BaseClass::Current() != uiNewSliderPos)
   {
      // current pos moved
      BaseClass::Current(uiNewSliderPos);
      SetAttr(ProgressBarAttr::Current, uiNewSliderPos);

      FireEvent(SliderEvent::Moved);
   }
}

bool Slider::OnKeyboardEvent(bool bKeyDown, unsigned int sym, unsigned int /*mod*/)
{
   if (!m_bFocused || !bKeyDown)
      return false;

   // check left and right keys
   bool bLeftKey = sym == SDLK_LEFT || sym == SDLK_UP || sym == SDLK_PAGEUP;
   bool bRightKey = sym == SDLK_RIGHT || sym == SDLK_DOWN || sym == SDLK_PAGEDOWN;

   if (!bLeftKey && !bRightKey)
      return false;

   unsigned int uiNewSliderPos = BaseClass::Current();

   if (bLeftKey && uiNewSliderPos > 0)
      uiNewSliderPos--;
   else
      if (bRightKey && uiNewSliderPos + 1 <= BaseClass::Max())
         uiNewSliderPos++;

   if (BaseClass::Current() != uiNewSliderPos)
   {
      // current pos moved
      BaseClass::Current(uiNewSliderPos);

      FireEvent(SliderEvent::Moved);
   }

   return true;
}

void Slider::OnFocusChanged(bool bGotFocus)
{
   m_bFocused = bGotFocus;
}

void Slider::OnAttributeChanged(const CString& cszAttributeName) throw()
{
   try
   {
      if (SliderAttr::SliderColor == cszAttributeName)
      {
         m_sliderColor = GetAttrAsColor(cszAttributeName);
      }
      else
         BaseClass::OnAttributeChanged(cszAttributeName);
   }
   catch (...)
   {
   }
}

bool Slider::IsInsideSliderArea(Point pt) const throw()
{
   try
   {
      Rect rect = Window::GetRect();
      rect.Add(Parent()->GetPos());

      unsigned int uiMid = DividerPoint() + rect.Left();
      rect.Left(uiMid - c_uiSliderWidth/2);
      rect.Right(uiMid + c_uiSliderWidth/2);

      return rect.IsInside(pt);
   }
   catch (...)
   {
      return false;
   }
}

unsigned int Slider::SliderPosByOffset(unsigned int uiAbsPosX)
{
   Rect rect = Window::GetRect();

   if (uiAbsPosX < unsigned(rect.Left()))
      return 0;

   if (uiAbsPosX >= unsigned(rect.Right()))
      return BaseClass::Max();

   // else calculate
   unsigned int uiRelPos = uiAbsPosX - rect.Left();

   double dPos = double(uiRelPos) / rect.Width();

   // add rounding factor
   dPos += 0.5 / BaseClass::Max();

   unsigned int uiSliderPos = static_cast<unsigned int>(dPos * BaseClass::Max());

   return uiSliderPos;
}
