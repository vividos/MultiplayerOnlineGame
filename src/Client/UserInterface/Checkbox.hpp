//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Checkbox.hpp Checkbox control
//
#pragma once

// includes
#include "UserInterfaceCommon.hpp"
#include "Control.hpp"
#include "TextTexture.hpp"

/// Checkbox attributes
namespace CheckboxAttr
{
   static TCHAR Text[] = _T("Text");   ///< text
   static TCHAR Color[] = _T("Color"); ///< text color
   static TCHAR BackColor[] = _T("BackColor"); ///< background color
   static TCHAR CheckColor[] = _T("CheckColor"); ///< check color
   static TCHAR BoxColor[] = _T("BoxColor"); ///< box color
   static TCHAR Checked[] = _T("Checked"); ///< check status
};

/// Checkbox events
namespace CheckboxEvent
{
   static TCHAR Clicked[] = _T("clicked"); ///< event that is fired when checkbox has been clicked/changed
};

/// checkbox control
class USERINTERFACE_DECLSPEC Checkbox : public Control
{
public:
   /// ctor
   Checkbox()
   {
   }
   /// dtor
   virtual ~Checkbox() {}

   BEGIN_ATTR_MAP()
      INHERIT_ATTR_MAP(Control)
      ATTR_ENTRY(CheckboxAttr::Text, _T(""))
      ATTR_ENTRY(CheckboxAttr::Color, _T("#FFFFFF"))
      ATTR_ENTRY(CheckboxAttr::BackColor, _T("#00000000"))
      ATTR_ENTRY(CheckboxAttr::CheckColor, _T("#00FF00"))
      ATTR_ENTRY(CheckboxAttr::BoxColor, _T("#FFFFFF"))
      ATTR_ENTRY(CheckboxAttr::Checked, _T("false"))
   END_ATTR_MAP()

   /// returns if checkbox is checked
   bool IsChecked() const
   {
      return GetAttrAsBool(CheckboxAttr::Checked);
   }

   /// sets check
   void SetCheck(bool bChecked)
   {
      SetAttrBool(CheckboxAttr::Checked, bChecked);
   }

   virtual void Render(Rect& rectArea) override;

   virtual bool OnMouseButtonEvent(bool /*bPressed*/, int /*iMouseButton*/, unsigned int /*x*/, unsigned int /*y*/) override;
   virtual void OnMouseMotionEvent(unsigned int /*x*/, unsigned int /*y*/) override {}
   virtual bool OnKeyboardEvent(bool /*bKeyDown*/, unsigned int /*sym*/, unsigned int /*mod*/) override;

   virtual void OnAttributeChanged(const CString& cszAttributeName) override;

private:
   /// updates texture
   void UpdateTexture();

   /// renders box and check
   void RenderCheck(Rect& rectArea);

private:
   /// texture
   TextTexture m_text;
};
