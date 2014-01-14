//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file version.h project version defines
//
#pragma once

#define MAIN_VERSION 1
#define MINOR_VERSION 0
#define SUB_VERSION 0
#define BUILD_NUMBER 1001

#define VERSIONINFO_FILEVERSION_NUMERIC    MAIN_VERSION##, ##MINOR_VERSION##, ##SUB_VERSION##, ##BUILD_NUMBER
#define VERSIONINFO_PRODUCTVERSION_NUMERIC MAIN_VERSION##, ##MINOR_VERSION##, ##SUB_VERSION

#define STR(x) STR2(x)
#define STR2(x) #x

#define VERSIONINFO_FILEVERSION_STRING    STR(MAIN_VERSION) "." STR(MINOR_VERSION) "." STR(SUB_VERSION) "." STR(BUILD_NUMBER)
#define VERSIONINFO_PRODUCTVERSION_STRING STR(MAIN_VERSION) "." STR(MINOR_VERSION) "." STR(SUB_VERSION)

#define VERSIONINFO_COPYRIGHT    "Copyright 2008-2014 Michael Fink"
#define VERSIONINFO_COMPANY_NAME "Michael Fink"
#define VERSIONINFO_PRODUCTNAME  "MultiplayerOnlineGame"
