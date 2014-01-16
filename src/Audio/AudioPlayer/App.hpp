//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Audio/AudioPlayer/App.hpp Audio player application
//
#pragma once

/// Audio player application
class App
{
public:
   /// ctor
   App(HINSTANCE hInstance);
   /// dtor
   ~App();

   /// runs application
   int Run(LPCTSTR lpstrCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT);
};
