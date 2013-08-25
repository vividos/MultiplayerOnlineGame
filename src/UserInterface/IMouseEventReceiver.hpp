//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
/// \file IMouseEventReceiver.hpp Mouse event receiver interface
//
#pragma once

// includes

/// mouse button type
enum T_enMouseButtonType
{
   buttonLeft = 1,
   buttonMiddle = 2,
   buttonRight = 3,
   buttonWheelUp = 4,
   buttonWheelDown = 5
};

/// mouse event receiver interface
class IMouseEventReceiver: public boost::noncopyable
{
public:
   /// dtor
   virtual ~IMouseEventReceiver() throw() {}

   /// called on mouse button event
   virtual void OnMouseButtonEvent(bool bPressed, T_enMouseButtonType enButtonType, int x, int y) = 0;

   /// called on mouse motion event
   virtual void OnMouseMotionEvent(int x, int y, int xrel, int yrel) = 0;
};
