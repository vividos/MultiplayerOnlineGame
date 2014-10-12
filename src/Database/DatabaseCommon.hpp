//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file DatabaseCommon.hpp Database project include
//
#pragma once

// includes
#include "Base.hpp"

/// export macro
#ifdef DATABASE_EXPORTS
#define DATABASE_DECLSPEC __declspec(dllexport)
#else
#define DATABASE_DECLSPEC __declspec(dllimport)
#endif
