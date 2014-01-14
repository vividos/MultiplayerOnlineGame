//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Slider.hpp Progress bar control
//
#pragma once

// includes
#include "UserInterfaceCommon.hpp"
#include "ProgressBar.hpp"

/// Slider attributes
namespace SliderAttr
{
   static TCHAR SliderColor[] = _T("SliderColor"); ///< slider color
};

/// Slider events
namespace SliderEvent
{
   static TCHAR Moved[] = _T("moved"); ///< event that is fired when slider has moved
};

/// \brief slider control
/// \details inherits from ProgressBar, since it shares the look and
/// the only difference is the additional slider icon and slider event
/// handling.
class USERINTERFACE_DECLSPEC Slider: public ProgressBar
{
   /// base class
   typedef ProgressBar BaseClass;

public:
   /// ctor
   Slider() throw()
      :m_bMouseButtonDown(false),
       m_iDragOffset(0),
       m_bFocused(false)
   {
   }

   BEGIN_EVENT_MAP()
      EVENT_ENTRY(SliderEvent::Moved)
   END_EVENT_MAP()

   BEGIN_ATTR_MAP()
      INHERIT_ATTR_MAP(ProgressBar)
      ATTR_ENTRY(SliderAttr::SliderColor, _T("#2000ff"))
   END_ATTR_MAP()

   virtual void Render(Rect& rectArea) override;

   virtual bool OnMouseButtonEvent(bool bPressed, int iMouseButton, unsigned int x, unsigned int y) override;
   virtual void OnMouseMotionEvent(unsigned int x, unsigned int y) override;
   virtual bool OnKeyboardEvent(bool bKeyDown, unsigned int sym, unsigned int mod) override;
   virtual void OnFocusChanged(bool bGotFocus) override;

   virtual void OnAttributeChanged(const CString& cszAttributeName) throw() override;

private:
   /// determines if point is in slider area
   bool IsInsideSliderArea(Point pt) const throw();

   /// calculates slider pos by absolute x offset
   unsigned int SliderPosByOffset(unsigned int uiAbsPosX);

private:
   /// slider color
   Color m_sliderColor;

   /// indicates if mouse button is down
   bool m_bMouseButtonDown;

   /// drag offset
   int m_iDragOffset;

   /// indicates if slider is focused
   bool m_bFocused;
};
