//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file DefaultActionKeyDefs.cpp default action key definitions
//

// includes
#include "StdAfx.h"
#include "DefaultActionKeyDefs.hpp"
#include "KeyboardActionManager.hpp"

void DefaultActionKeyDefs::Register(KeyboardActionManager& mgr)
{
   // register some keys
   mgr.RegisterActionKey(_T("strg+f11"), KeyboardActionManager::actionToggleFullscreen);
   mgr.RegisterActionKey(_T("alt+x"), KeyboardActionManager::actionQuitApplication);
   mgr.RegisterActionKey(_T("1"), KeyboardActionManager::actionAction1);
   mgr.RegisterActionKey(_T("2"), KeyboardActionManager::actionAction2);
   mgr.RegisterActionKey(_T("tab"), KeyboardActionManager::actionSelectNext);
   mgr.RegisterActionKey(_T("shift+tab"), KeyboardActionManager::actionSelectPrev);

   // movement keys
   mgr.RegisterActionKey(_T("w"), KeyboardActionManager::actionMoveForward);
   mgr.RegisterActionKey(_T("s"), KeyboardActionManager::actionMoveBackward);
   mgr.RegisterActionKey(_T("a"), KeyboardActionManager::actionRotateLeft);
   mgr.RegisterActionKey(_T("d"), KeyboardActionManager::actionRotateRight);
   mgr.RegisterActionKey(_T("q"), KeyboardActionManager::actionMoveLeft);
   mgr.RegisterActionKey(_T("e"), KeyboardActionManager::actionMoveRight);
   mgr.RegisterActionKey(_T("numlock"), KeyboardActionManager::actionAutoMove);
}
