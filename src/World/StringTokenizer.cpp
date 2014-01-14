//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file StringTokenizer.cpp String tokenizer
//

// includes
#include "StdAfx.h"
#include "StringTokenizer.hpp"

CString StringTokenizer::Next()
{
   // brackets are own tokens, not separated by space
   CString cszTemp = m_cszText.Left(1);
   if (cszTemp == _T("[") ||
       cszTemp == _T("]"))
   {
      m_cszText.Delete(0);
      return cszTemp;
   }

   int iPos = m_cszText.Find(_T(' '));

   if (iPos == -1 && m_cszText.Right(1) == _T("]"))
   {
      iPos = m_cszText.GetLength()-1;
      m_cszText.Insert(iPos, _T(' '));
   }

   if (iPos == -1)
   {
      cszTemp = m_cszText;
      m_cszText.Empty();
      return cszTemp;
   }

   // when token ends with closing bracket, don't remove that now
   if (iPos > 0 && m_cszText[iPos-1] == _T(']'))
      iPos--;

   cszTemp = m_cszText.Left(iPos);

   m_cszText.Delete(0, iPos+1);

   return cszTemp;
}
