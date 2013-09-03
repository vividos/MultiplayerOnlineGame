//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file DefaultActionKeyDefs.hpp default action key definitions
//
#pragma once

// includes
#include "ClientLogic.hpp"

// forward references
class KeyboardActionManager;

/// default action key definitions
class CLIENTLOGIC_DECLSPEC DefaultActionKeyDefs: public boost::noncopyable
{
public:
   /// register key defs in keyboard action manager
   static void Register(KeyboardActionManager& mgr);

private:
   /// ctor
   DefaultActionKeyDefs();
};
