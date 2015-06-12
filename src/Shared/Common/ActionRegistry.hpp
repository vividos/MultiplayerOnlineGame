//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ActionRegistry.hpp Action registry
//
#pragma once

// includes
#include "Common.hpp"

/// \brief Action registry
/// Used to create action class instances by given id (T_enActionId)
class COMMON_DECLSPEC ActionRegistry
{
public:
   /// creates action instance by given id
   static ActionPtr CreateById(unsigned short usActionId);

private:
   /// ctor; not implemented
   ActionRegistry();
};
