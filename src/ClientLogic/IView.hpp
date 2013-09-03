//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file IView.hpp MVVM view interface
//
#pragma once

// includes
#include "IMouseEventReceiver.hpp"

// forward references
class Vector3d;

/// MVVM view interface
class IView: public IMouseEventReceiver
{
public:
   /// dtor
   virtual ~IView() throw() {}

   /// called before rendering
   virtual void PreRender() = 0;
};
