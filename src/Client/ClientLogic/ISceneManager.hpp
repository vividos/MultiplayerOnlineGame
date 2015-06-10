//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ISceneManager.hpp Scene manager interface
//
#pragma once

// includes
#include <functional>

// forward references
class Scene;
class Point;
class Size;

/// scene manager interface
class ISceneManager
{
public:
   /// dtor
   virtual ~ISceneManager() throw()
   {
   }

   /// called to change current scene
   virtual void ChangeScene(std::shared_ptr<Scene> spScene) = 0;

   /// dispatches handler in the thread that runs the event loop
   virtual void DispatchInEventLoop(std::function<void()> fn) = 0;

   /// returns mouse position
   virtual Point GetMousePos() throw() = 0;

   /// sets new mouse position
   virtual void SetMousePos(const Point& pt) throw() = 0;

   /// shows or hides mouse cursor
   virtual void ShowCursor(bool bShow) throw() = 0;

   /// returns current screen size in pixels
   virtual Size GetScreenSize() const throw() = 0;
};
