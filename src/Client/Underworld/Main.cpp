//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Main.cpp main function
//

// includes
#include "stdafx.h"
#include "GameClient.hpp"

/// main function
int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
   try
   {
      GameClient gc;
      gc.Start();
      //gc.Decompile();
   }
   catch(Exception& ex)
   {
      CString cszText;
      cszText.Format(_T("Underworld ended with exception: %s"), ex.Message().GetString());
      MessageBox(GetActiveWindow(), cszText, _T("Underworld"), MB_OK);
   }

   return 0;
}
