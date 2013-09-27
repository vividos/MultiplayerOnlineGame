//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
/// \file Underworld/UwBase.hpp Underworld base header
//
#pragma once

// includes
#include <SDL.h>
#include <ulib/Exception.hpp>
#include "Constants.hpp"

#include <vector>
#include <set>
#include <map>

// defines

#define UaAssert ATLASSERT
//#define UaTrace(x) LOG_INFO(##x, "Underworld")
#define UaTrace ATLTRACE

// forward references
namespace UwBase
{
   class Savegame;
}
