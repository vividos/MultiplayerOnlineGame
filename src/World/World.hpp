//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file World.hpp World project include
//
#pragma once

// includes
#include "Base.hpp"
#include "Common.hpp"

/// export macro
#ifdef WORLD_EXPORTS
#define WORLD_DECLSPEC __declspec(dllexport)
#else
#define WORLD_DECLSPEC __declspec(dllimport)
#endif
