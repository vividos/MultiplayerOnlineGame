//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ProgressBar.hpp Progress bar control
//
#pragma once

// includes
#include "UserInterfaceCommon.hpp"
#include "Control.hpp"

/// ProgressBar attributes
namespace ProgressBarAttr
{
   static TCHAR Current[] = _T("Current");      ///< current value
   static TCHAR Max[] = _T("Max");              ///< max value
   static TCHAR Color[] = _T("Color");          ///< bar color
   static TCHAR BackColor[] = _T("BackColor");  ///< background color
};

/// ProgressBar events
namespace ProgressBarEvent
{
};

/// progress bar control
class USERINTERFACE_DECLSPEC ProgressBar: public Control
{
public:
   /// ctor
   ProgressBar()
      :m_uiPos(0),
       m_uiMax(100)
   {
   }

   // get methods

   /// returns current pos
   unsigned int Current() const { return m_uiPos; }

   /// returns max pos
   unsigned int Max() const { return m_uiMax; }

   // set methods

   /// sets new current pos
   void Current(unsigned int uiPos) { m_uiPos = uiPos; }

   /// sets new max pos
   void Max(unsigned int uiMax) { m_uiMax = uiMax; }

   BEGIN_EVENT_MAP()
   END_EVENT_MAP()

   BEGIN_ATTR_MAP()
      INHERIT_ATTR_MAP(Control)
      ATTR_ENTRY(ProgressBarAttr::Current, _T("0"))
      ATTR_ENTRY(ProgressBarAttr::Max, _T("100"))
      ATTR_ENTRY(ProgressBarAttr::Color, _T("#ffffff"))
      ATTR_ENTRY(ProgressBarAttr::BackColor, _T("#000000"))
   END_ATTR_MAP()

   virtual void Render(Rect& rectArea) override;

   virtual void OnAttributeChanged(const CString& cszAttributeName) override;

protected:
   /// calculates divider point; relative to x pos
   unsigned int DividerPoint() const;

private:
   /// position
   unsigned int m_uiPos;

   /// max. position
   unsigned int m_uiMax;

   /// bar color
   Color m_color;

   /// bar background color
   Color m_backColor;
};
