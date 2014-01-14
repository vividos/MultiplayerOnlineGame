//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file WorldBuilder.cpp World builder main function
//

// includes
#include "stdafx.h"
#include "App.hpp"

/// main function
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
   App app(hInstance);
   return app.Run(lpstrCmdLine, nCmdShow);
}
