//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file FontManager.hpp Font manager class
//
#pragma once

// includes
#include "UserInterfaceCommon.hpp"
#include "Font.hpp"

/// font typeface names
namespace FontTypeface
{
   static TCHAR Normal[] = _T("Tahoma");        ///< normal font typeface
   static TCHAR Monospace[] = _T("Courier New");///< monospace font typeface
};

/// font manager
class USERINTERFACE_DECLSPEC FontManager: public boost::noncopyable
{
public:
   /// ctor
   FontManager();

   /// creates a new font
   FontPtr Create(const CString& cszTypeface = FontTypeface::Normal, unsigned int uiHeight = 12);

private:
   /// implementation
   std::shared_ptr<FontManagerImpl> m_spImpl;
};
