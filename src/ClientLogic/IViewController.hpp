//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file IViewController.hpp View controller interface
//
#pragma once

// includes
#include "IMouseEventReceiver.hpp"

/// view controller interface
class IViewController: public IMouseEventReceiver
{
public:
   /// dtor
   virtual ~IViewController() throw() {}

   /// tick processing
   virtual void Tick() = 0;
};
