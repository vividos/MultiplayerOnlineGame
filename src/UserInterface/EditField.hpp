//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
/// \file EditField.hpp Edit field control
//
#pragma once

// includes
#include "UserInterfaceCommon.hpp"
#include "Control.hpp"
#include "TextTexture.hpp"

/// EditField attributes
namespace EditFieldAttr
{
   static TCHAR Text[] = _T("Text");         ///< edit text
   static TCHAR Readonly[] = _T("Readonly"); ///< attribute that is set to true to prevent changes to field
   static TCHAR Password[] = _T("Password"); ///< attribute that is set to true to get a password edit field
};

/// EditField events
namespace EditFieldEvent
{
   static TCHAR Changed[] = _T("changed"); ///< event that is fired when text was changed
};

/// edit field control
class USERINTERFACE_DECLSPEC EditField: public Control
{
public:
   /// ctor
   EditField() throw()
      :m_uiCaretPos(0),
       m_uiCaretX(0),
       m_uiCaretHeight(0),
       m_bFocused(false)
   {
   }

   BEGIN_EVENT_MAP()
      EVENT_ENTRY(EditFieldEvent::Changed)
   END_EVENT_MAP()

   BEGIN_ATTR_MAP()
      INHERIT_ATTR_MAP(Control)
      ATTR_ENTRY(EditFieldAttr::Text, _T(""))
      ATTR_ENTRY(EditFieldAttr::Readonly, _T("false"))
      ATTR_ENTRY(EditFieldAttr::Password, _T("false"))
   END_ATTR_MAP()

   virtual void Render(Rect& rectArea) override;

   virtual bool OnMouseButtonEvent(bool bPressed, int iMouseButton, unsigned int x, unsigned int y) override;
   virtual void OnMouseMotionEvent(unsigned int x, unsigned int y) override;
   virtual bool OnKeyboardEvent(bool bKeyDown, unsigned int sym, unsigned int mod) override;
   virtual void OnFocusChanged(bool bGotFocus) override;

   virtual void OnAttributeChanged(const CString& cszAttributeName) throw() override;

   /// virtual function that can be implemented to filter for allowed characters
   virtual bool IsAllowedChar(unsigned int sym, unsigned int mod);

   /// returns display text
   CString GetDisplayText() const;

private:
   /// updates edit field texture
   void UpdateTexture();
   /// updates caret position
   void UpdateCaretPos();

private:
   /// texture
   TextTexture m_text;

   /// text for edit control
   CString m_cszText;

   /// logical caret position
   unsigned int m_uiCaretPos;

   /// caret x coordinate
   unsigned int m_uiCaretX;

   /// caret height
   unsigned int m_uiCaretHeight;

   /// indicates if control is focused
   bool m_bFocused;
};
