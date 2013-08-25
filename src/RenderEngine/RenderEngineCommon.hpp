//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file RenderEngineCommon.hpp RenderEngine project include
//
#pragma once

// includes
#include "Base.hpp"

/// export macro
#ifdef RENDERENGINE_EXPORTS
#  define RENDERENGINE_DECLSPEC __declspec(dllexport)
#else
#  define RENDERENGINE_DECLSPEC __declspec(dllimport)
#endif
