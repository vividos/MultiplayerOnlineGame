//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Font.hpp Font class
//
#pragma once

// includes
#include "UserInterfaceCommon.hpp"
#include "Color.hpp"

// forward references
class Bitmap;
class FontImpl;
class FontManagerImpl;

/// \brief single font
class USERINTERFACE_DECLSPEC Font: public boost::noncopyable
{
public:
   // get methods

   /// returns the vertical distance from the horizontal baseline to the highest character coordinate
   unsigned int Ascender();

   /// returns the vertical distance from the horizontal baseline to the lowest character coordinate
   unsigned int Descender();

   /// height; means a default line spacing, i.e. a baseline-to-baseline distance
   unsigned int Height();

   // methods

   /// returns width of resulting string
   unsigned int MeasureString(const CString& cszText);

   /// returns string length that would fit into given width
   unsigned int MeasureTextLength(const CString& cszText, unsigned int uiWidth);

   /// writes text to bitmap
   void Write(const CString& cszText, Bitmap& bmp, unsigned int x, unsigned int y, Color cForeground, Color cBackground);

private:
   friend class FontManager;

   /// ctor
   Font(FontImpl* pImpl, std::shared_ptr<FontManagerImpl> spManagerImpl);

   /// font manager impl
   /// \note must come before m_spImpl, so that it is cleaned up after font impl
   std::shared_ptr<FontManagerImpl> m_spManagerImpl;

   /// font impl
   std::shared_ptr<FontImpl> m_spImpl;
};

/// font smart ptr
typedef std::shared_ptr<Font> FontPtr;
