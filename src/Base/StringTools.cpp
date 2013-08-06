//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file StringTools.cpp String tools
//

// includes
#include "StdAfx.h"
#include "StringTools.hpp"

void StringTools::StringToUTF8(const CString& cszText, std::vector<unsigned char>& vecUtf8Text)
{
#ifdef _UNICODE
   LPCWSTR pszwText = cszText;
#else
   // since we get an ANSI string, convert to wide character string beforehand
   USES_CONVERSION;
   LPCWSTR pszwText = T2CW(cszText);
#endif

   // get size
   int iSize = ::WideCharToMultiByte(CP_UTF8, 0, pszwText, -1, NULL, 0, NULL, NULL);

   vecUtf8Text.resize(iSize);

   // convert to utf-8
   LPSTR pszaOut = reinterpret_cast<LPSTR>(&vecUtf8Text[0]);

   ATLVERIFY(iSize == ::WideCharToMultiByte(CP_UTF8, 0, pszwText, -1, pszaOut, iSize, NULL, NULL));
}

/// converts next two hex digits in string to hex
unsigned char HexDigitsToByte(LPCTSTR pszHex)
{
   ATLASSERT(_tcslen(pszHex) >= 2);

   CString cszBytes(pszHex, 2);
   // TODO replace with faster function
   return static_cast<unsigned char>(_tcstoul(cszBytes, NULL, 16) & 0xFF);
}

void StringTools::HexStringToBytes(const CString& cszText, std::vector<unsigned char>& vecData)
{
   int iLen = cszText.GetLength();
   ATLASSERT((iLen & 1) == 0); // must be even
   for (int i=0; i<iLen; i+=2)
      vecData.push_back(HexDigitsToByte(cszText.GetString()+i));
}
