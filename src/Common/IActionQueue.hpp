//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file IActionQueue.hpp action queue interface
//
#pragma once

// includes
#include "Action.hpp"

/// action queue interface
class IActionQueue
{
public:
   /// dtor
   virtual ~IActionQueue() throw() {}

   /// post action
   virtual void Post(ActionPtr spAction) = 0;
};
