//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file CommonDefines.hpp Common defines
//
#pragma once

/// game name; used for captions, folder names, etc.
static const TCHAR c_pszGameName[] = _T("Multiplayer Online Game");

/// default server hostname
static const TCHAR c_pszDefaultServerHostname[] = _T("localhost");

/// default game server port
const unsigned short c_usDefaultServerPort = 8452;

/// maximum selection distance from player, in units
const double c_dMaxSelectionDistance = 50.0;

/// maximum visibility distance for objects, in units
const double c_dMaxVisibleDistance = 250.0;

/// path name of folder for crash dumps, used in CrashReporter::Init()
static const TCHAR c_pszCrashdumpFoldername[] = _T("crashdumps");
