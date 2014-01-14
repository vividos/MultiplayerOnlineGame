//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Audio.hpp Audio project include
//
#pragma once

// includes
#define ULIB_ASIO_NO_WINSOCK
#include "Base.hpp"

/// export macro
#ifdef AUDIO_EXPORTS
#define AUDIO_DECLSPEC __declspec(dllexport)
#else
#define AUDIO_DECLSPEC __declspec(dllimport)
#endif
