//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
/// \file TextField.hpp Text field control
//
#pragma once

// includes
#include "UserInterfaceCommon.hpp"
#include "Control.hpp"
#include "TextTexture.hpp"

/// TextField attributes
namespace TextFieldAttr
{
   static TCHAR Text[] = _T("Text");   ///< text
   static TCHAR Color[] = _T("Color"); ///< text color
};

/// static text field control
class USERINTERFACE_DECLSPEC TextField: public Control
{
public:
   /// ctor
   TextField() throw()
   {
   }

   BEGIN_ATTR_MAP()
      INHERIT_ATTR_MAP(Control)
      ATTR_ENTRY(TextFieldAttr::Text, _T(""))
      ATTR_ENTRY(TextFieldAttr::Color, _T("#FFFFFF"))
   END_ATTR_MAP()

   virtual void Render(Rect& rectArea) override;

   virtual bool OnMouseButtonEvent(bool /*bPressed*/, int /*iMouseButton*/, unsigned int /*x*/, unsigned int /*y*/) override { return false; }
   virtual void OnMouseMotionEvent(unsigned int /*x*/, unsigned int /*y*/) override {}
   virtual bool OnKeyboardEvent(bool /*bKeyDown*/, unsigned int /*sym*/, unsigned int /*mod*/) override { return false; }

   virtual void OnAttributeChanged(const CString& cszAttributeName) throw() override;

private:
   /// updates texture
   void UpdateTexture();

private:
   /// texture
   TextTexture m_text;
};
