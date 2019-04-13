//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file KeyboardActionBindings.hpp Arena view
//
#pragma once

// includes
#include "KeyboardActionManager.hpp"

/// collection of keyboard action bindings
class KeyboardActionBindings
{
public:
   /// ctor
   KeyboardActionBindings(KeyboardActionManager& keyboardActionManager)
      :m_keyboardActionManager(keyboardActionManager)
   {
   }

   /// registers action handler and stores key action handle internally; press-only version
   void RegisterActionHandler(KeyboardActionManager::T_enAction enAction,
      KeyboardActionManager::T_fnOnKeyboardAction fnOnKeyboardAction)
   {
      KeyActionHandlePtr spHandle =
         m_keyboardActionManager.RegisterActionHandler(enAction, fnOnKeyboardAction);

      m_setAllActions.insert(spHandle);
   }

   /// registers action handler and stores key action handle internally; press-or-release version
   void RegisterActionHandler(KeyboardActionManager::T_enAction enAction, bool bOnPressed,
      KeyboardActionManager::T_fnOnKeyboardActionKeyEvent fnOnKeyboardActionKeyEvent)
   {
      KeyActionHandlePtr spHandle =
         m_keyboardActionManager.RegisterActionHandler(enAction, bOnPressed, fnOnKeyboardActionKeyEvent);

      m_setAllActions.insert(spHandle);
   }

private:
   /// keyboard action manager
   KeyboardActionManager& m_keyboardActionManager;

   /// all stored key action handles
   std::set<KeyActionHandlePtr> m_setAllActions;
};
