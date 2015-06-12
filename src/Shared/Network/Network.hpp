//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Network/Network.hpp Network project include
//
#pragma once

// includes
#include "Base.hpp"

/// export macro
#ifdef NETWORK_EXPORTS
#  define NETWORK_DECLSPEC __declspec(dllexport)
#else
#  define NETWORK_DECLSPEC __declspec(dllimport)
#endif
