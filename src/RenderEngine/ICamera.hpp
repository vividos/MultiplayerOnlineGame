//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ICamera.hpp Camera interface
//
#pragma once

/// camera interface
class ICamera
{
public:
   /// dtor
   virtual ~ICamera()
   {
   }

   /// uses camera
   virtual void Use() = 0;
};
