//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
/// \file Button.hpp Button control
//
#pragma once

// includes
#include "UserInterfaceCommon.hpp"
#include "Control.hpp"
#include "TextTexture.hpp"

/// Button attributes
namespace ButtonAttr
{
   static TCHAR Text[] = _T("Text");   ///< button text
};

/// Button events
namespace ButtonEvent
{
   static TCHAR Clicked[] = _T("clicked");   ///< event that is fired when button was pressed
};


/// button control
class USERINTERFACE_DECLSPEC Button: public Control
{
public:
   /// ctor
   Button() throw()
      :m_bStatePressed(false)
   {
   }

   BEGIN_EVENT_MAP()
      EVENT_ENTRY(ButtonEvent::Clicked)
   END_EVENT_MAP()

   BEGIN_ATTR_MAP()
      INHERIT_ATTR_MAP(Control)
      ATTR_ENTRY(ButtonAttr::Text, _T(""))
   END_ATTR_MAP()

   virtual void Render(Rect& rectArea);

   virtual bool OnMouseButtonEvent(bool bPressed, int iMouseButton, unsigned int x, unsigned int y) override;
   virtual void OnMouseMotionEvent(unsigned int x, unsigned int y) override;
   virtual bool OnKeyboardEvent(bool bKeyDown, unsigned int sym, unsigned int mod) override;

   virtual void OnAttributeChanged(const CString& cszAttributeName) throw() override;

private:
   /// called when mouse left the button area when clicked
   void OnMouseLeft();

   /// updates button texture
   void UpdateTexture();

private:
   /// indicates pressed button state
   bool m_bStatePressed;

   /// texture for button text
   TextTexture m_text;
};
