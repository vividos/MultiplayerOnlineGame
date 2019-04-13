//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file IView.hpp MVVM view interface
//
#pragma once

// includes
#include "IMouseEventReceiver.hpp"
#include "Vector3.hpp"

/// MVVM view interface
class IView: public IMouseEventReceiver
{
public:
   /// dtor
   virtual ~IView() {}

   /// called on mouse button event
   virtual void OnMouseButtonEvent(bool /*bPressed*/, T_enMouseButtonType /*enButtonType*/, int /*x*/, int /*y*/) override {}

   /// called on mouse motion event
   virtual void OnMouseMotionEvent(int /*x*/, int /*y*/, int /*xrel*/, int /*yrel*/) override {}

   /// called before rendering
   virtual void PreRender() = 0;
};
