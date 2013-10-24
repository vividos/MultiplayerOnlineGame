//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ServerLogic.hpp ServerLogic project include
//
#pragma once

// includes
#include "Base.hpp"
#include "Common.hpp"

/// export macro
#ifdef SERVERLOGIC_EXPORTS
#define SERVERLOGIC_DECLSPEC __declspec(dllexport)
#else
#define SERVERLOGIC_DECLSPEC __declspec(dllimport)
#endif
