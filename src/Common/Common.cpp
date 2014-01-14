//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Common.cpp DLL main function
//

// includes
#include "stdafx.h"
#include <windows.h>

#ifdef _MANAGED
#pragma managed(push, off)
#endif

/// DLL main function
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved)
{
   hModule; dwReasonForCall; lpReserved;
   return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif
