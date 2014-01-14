//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file RenderEngine/stdafx.h Precompiled header support
//
#pragma once

// compile for Windows 7 target to have Ribbon code available
#define _WIN32_WINNT _WIN32_WINNT_WIN7

// includes
#include <ulib/config/Wtl.hpp>

// additional WTL includes
#define _WTL_NO_WTYPES
#define max(x,y) ((x) > (y) ? (y) : (x))
#include <atlribbon.h>
#undef max

// game includes
#pragma warning(disable: 4005) // '_WIN32_WINNT' : macro redefinition
#define ULIB_ASIO_NO_WINSOCK
#include "Base.hpp"
