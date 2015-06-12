//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Base.Unit/stdafx.h Precompiled header support
//
#pragma once

// includes
#define ULIB_ASIO_NO_WINSOCK
#include <ulib/config/AutoLink.hpp>
#undef ULIB_DECLSPEC
#define ULIB_DECLSPEC

//#pragma warning(disable: 4273) // inconsistent dll linkage
#include "Base.hpp"
#include "CppUnitTest.h"

/// \brief Unit test classes
namespace UnitTest
{
}
