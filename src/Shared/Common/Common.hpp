//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Common/Common.hpp Common project include
//
#pragma once

// includes
#include "Base.hpp"
#include "CommonDefines.hpp"
#include "LogCategories.hpp"

// forward references
class Object;
class Mobile;
class Action;

// typedefs
typedef std::shared_ptr<Object> ObjectPtr; ///< Object smart ptr
typedef std::shared_ptr<Mobile> MobilePtr; ///< Mobile smart ptr
typedef std::shared_ptr<Action> ActionPtr; ///< Action smart ptr

/// export macro
#ifdef COMMON_EXPORTS
#  define COMMON_DECLSPEC __declspec(dllexport)
#else
#  define COMMON_DECLSPEC __declspec(dllimport)
#endif
