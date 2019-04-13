//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TextTexture.hpp Texture that contains text
//
#pragma once

// includes
#include "Color.hpp"
#include "SizePointRect.hpp"
#include "Texture.hpp"

// forward references
class Font;

/// contains texture for a text string
class TextTexture
{
public:
   /// text alignment
   enum T_enTextAlignment
   {
      textAlignLeft = 0,   ///< align left
      textAlignCenter = 1, ///< align centered
   };

   /// returns if texture is already initialized
   bool IsInit() const { return m_tex.Size() != 0; }

   /// renders texture quad
   void Render(Rect rc);

   /// updates texture text
   void UpdateText(Font& font, const CString& cszText, const Size& s,
      T_enTextAlignment enTextAlignment,
      Color cForeground, Color cBackground);

private:
   /// texture
   Texture m_tex;
};
