//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TextField.cpp Text field control
//

// includes
#include "StdAfx.h"
#include "TextField.hpp"
#include "IWindowManager.hpp"

void TextField::Render(Rect& rectArea)
{
   if (!m_text.IsInit())
      UpdateTexture();

   m_text.Render(rectArea);
}

void TextField::OnAttributeChanged(const CString& cszAttributeName) throw()
{
   if (Parent() == NULL)
      return;

   if (cszAttributeName == TextFieldAttr::Text ||
       cszAttributeName == TextFieldAttr::BackColor ||
       cszAttributeName == TextFieldAttr::Color)
      UpdateTexture();
}

void TextField::UpdateTexture()
{
   Font& font = GetFont();

   Size s = GetSize();
   CString cszText = GetAttr(TextFieldAttr::Text);
   Color cText = GetAttrAsColor(TextFieldAttr::Color);
   Color cBack = GetAttrAsColor(TextFieldAttr::BackColor);

   m_text.UpdateText(font, cszText, s, TextTexture::textAlignLeft, cText, cBack);
}
