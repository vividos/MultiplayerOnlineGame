//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
/// \file Slider.cpp Slider control
//

// includes
#include "StdAfx.h"
#include "Slider.hpp"

/// percent of height that consists of the bar
const double c_dBarHeightPercent = 0.3;

/// width of slider
const unsigned int c_uiSliderWidth = 30;

/// square root of 2, from Wolfram Alpha
const double c_dSquareRoot2 = 1.4142135623730950488016887242096980785696718753769480;

void Slider::Render(Rect& rectArea) throw()
{
   // render progress bar part
   {
      Rect rectBar(rectArea);
      rectBar.Top(rectBar.Bottom() - int(rectBar.Height() * c_dBarHeightPercent));

      BaseClass::Render(rectBar);
   }

   // render slider
   {
      int iMid = rectArea.Left() + int(DividerPoint());

      double dAspect = 1.33333;

      unsigned int uiRemainingHeight = unsigned(rectArea.Height() * (1.0 - c_dBarHeightPercent));
      unsigned int y = static_cast<unsigned int>((c_uiSliderWidth * dAspect/2.0 ) / c_dSquareRoot2);
      unsigned int x = (uiRemainingHeight < y) ? 0 : (uiRemainingHeight - y);

      for (int i=0; i<2; i++)
      {
         glBegin(i == 0 ? GL_POLYGON : GL_LINE_LOOP);

            if (i == 0)
               glColor4ubv(m_sliderColor.m_color);
            else
               glColor3ub(0, 0, 0);

            glVertex2i(iMid-c_uiSliderWidth/2, rectArea.Top());
            glVertex2i(iMid-c_uiSliderWidth/2, rectArea.Top()+x);
            glVertex2i(iMid,                   rectArea.Top()+uiRemainingHeight);
            glVertex2i(iMid+c_uiSliderWidth/2, rectArea.Top()+x);
            glVertex2i(iMid+c_uiSliderWidth/2, rectArea.Top());
         glEnd();
      }
   }
}

bool Slider::OnMouseButtonEvent(bool bPressed, int /*iMouseButton*/, unsigned int x, unsigned int y)
{
   if (bPressed && IsInsideSliderArea(Point(x, y)))
   {
      // start sliding
      m_bMouseButtonDown = true;

      // calc drag x offset
      Rect rect = Window::GetRect();
      unsigned int uiMid = DividerPoint();

      m_iDragOffset = x - int(rect.Left() + uiMid);
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
   unsigned int uiNewSliderPos = SliderPosByOffset(x + m_iDragOffset);

   if (BaseClass::Current() != uiNewSliderPos)
   {
      // current pos moved
      BaseClass::Current(uiNewSliderPos);

      FireEvent(SliderEvent::Moved);
   }
}

bool Slider::OnKeyboardEvent(bool /*bKeyDown*/, unsigned int /*sym*/, unsigned int /*mod*/)
{
   // TODO
   // check left and right keys
   return false;
}

void Slider::OnFocusChanged(bool bGotFocus)
{
   m_bFocused = bGotFocus;
}

void Slider::OnAttributeChanged(const CString& cszAttributeName) throw()
{
   if (SliderAttr::SliderColor == cszAttributeName)
   {
      m_sliderColor = GetAttrAsColor(cszAttributeName);
   }
   else
      BaseClass::OnAttributeChanged(cszAttributeName);
}

bool Slider::IsInsideSliderArea(Point pt) const throw()
{
   Rect rect = Window::GetRect();
   rect.Add(Parent()->GetPos());

   unsigned int uiMid = DividerPoint() + rect.Left();
   rect.Left(uiMid - c_uiSliderWidth/2);
   rect.Right(uiMid + c_uiSliderWidth/2);

   return rect.IsInside(pt);
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
