//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Common/Common.hpp Common project include
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
typedef boost::shared_ptr<Object> ObjectPtr; ///< Object smart ptr
typedef boost::shared_ptr<Mobile> MobilePtr; ///< Mobile smart ptr
typedef boost::shared_ptr<Action> ActionPtr; ///< Action smart ptr

/// export macro
#ifdef COMMON_EXPORTS
#  define COMMON_DECLSPEC __declspec(dllexport)
#else
#  define COMMON_DECLSPEC __declspec(dllimport)
#endif
