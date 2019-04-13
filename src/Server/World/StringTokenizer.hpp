//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file StringTokenizer.hpp String tokenizer
//
#pragma once

/// tokenizes strings separated by space char or brackets [ and ]
class StringTokenizer
{
public:
   /// ctor
   StringTokenizer(LPCTSTR pszText = _T(""))
      :m_cszText(pszText)
   {
   }

   /// gets next token
   CString Next();

   /// returns if tokenized string is already empty
   bool IsEmpty() const { return m_cszText.IsEmpty(); }

private:
   /// string to tokenize
   CString m_cszText;
};
