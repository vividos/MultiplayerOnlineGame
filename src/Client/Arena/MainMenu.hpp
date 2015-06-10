//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MainMenu.hpp Main menu dialog
//
#pragma once

// includes
#include "Panel.hpp"

// forward references
class IWindowManager;
class IFileSystem;

/// main menu
class MainMenu : public Panel
{
   typedef Panel BaseClass;

public:
   MainMenu(IWindowManager& windowManager, IFileSystem& fileSystem);
   virtual ~MainMenu() {}

   void Show(Window& parent);
   void Hide();

private:
   // virtual methdods from Window

   virtual bool OnKeyboardEvent(bool /*bKeyDown*/, unsigned int /*sym*/, unsigned int /*mod*/);
};

