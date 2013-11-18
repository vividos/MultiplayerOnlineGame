//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file MainMenu.cpp Main menu dialog
//

// includes
#include "stdafx.h"
#include "MainMenu.hpp"
#include "WindowManager.hpp"
#include "IFileSystem.hpp"
#include <SDL.h>

MainMenu::MainMenu(IWindowManager& windowManager, IFileSystem& fileSystem)
{
   WindowManager::CreateFromJson(windowManager.GetRootWindow(), fileSystem, _T("ui/mainmenu.json"));
}

void MainMenu::Show(Window& parent)
{
   BaseClass::Create(parent);
}

void MainMenu::Hide()
{
   BaseClass::Destroy();
}

bool MainMenu::OnKeyboardEvent(bool bKeyDown, unsigned int sym, unsigned int /*mod*/)
{
   if (bKeyDown && sym == SDLK_ESCAPE)
   {
      Hide();
      return true;
   }

   return false;
}
