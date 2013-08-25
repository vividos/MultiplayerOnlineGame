//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
/// \file UserInterfaceCommon.hpp UserInterface project include
//
#pragma once

// includes
#include "Common.hpp"

/// export macro
#ifdef USERINTERFACE_EXPORTS
#  define USERINTERFACE_DECLSPEC __declspec(dllexport)
#else
#  define USERINTERFACE_DECLSPEC __declspec(dllimport)
#endif
