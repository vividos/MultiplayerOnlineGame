//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Model3DCommon.hpp Model3D project include
//
#pragma once

// includes
#include "Base.hpp"

/// export macro
#ifdef MODEL3D_EXPORTS
#  define MODEL3D_DECLSPEC __declspec(dllexport)
#else
#  define MODEL3D_DECLSPEC __declspec(dllimport)
#endif
