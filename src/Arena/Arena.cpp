//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Arena.cpp Main function
//

// includes
#include "stdafx.h"
#include "GameClient.hpp"
#include "CrashReporter.hpp"
#include "Filesystem.hpp"

/// Arena main function
int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
   CrashReporter::Init(Filesystem().UserFolder() + c_pszCrashdumpFoldername);

   try
   {
      Arena::GameClient gc;
      gc.Start();
   }
   catch(Exception& ex)
   {
      CString cszText;
      cszText.Format(_T("Arena ended with exception: %s"), ex.Message());
      MessageBox(GetActiveWindow(), cszText, _T("Arena"), MB_OK);
   }

   return 0;
}
