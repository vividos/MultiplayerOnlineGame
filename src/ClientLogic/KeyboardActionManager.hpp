//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file KeyboardActionManager.hpp Keyboard action manager
//
#pragma once

// includes
#include "ClientLogic.hpp"
#include <map>
#include <set>
#include <functional>
#include <boost/noncopyable.hpp>

// forward references
class KeyActionHandle;

/// handle to registered key action
typedef std::shared_ptr<KeyActionHandle> KeyActionHandlePtr;

/// \brief manages key presses and starts actions on key presses
class CLIENTLOGIC_DECLSPEC KeyboardActionManager: public boost::noncopyable
{
public:
   DEFINE_INSTANCE(KeyboardActionManager)

   /// enum with all actions that can be triggered through key presses
   enum T_enAction
   {
      // misc. actions
      actionQuitApplication,  ///< quits application
      actionToggleFullscreen, ///< toggles fullscreen

      // moving the player character
      actionMoveForward,   ///< move forward
      actionMoveBackward,  ///< move backward
      actionMoveLeft,      ///< strafe left
      actionMoveRight,     ///< strafe right
      actionRotateLeft,    ///< rotate view left
      actionRotateRight,   ///< rotate view right
      actionAutoMove,      ///< toggles auto move forward

      // moving view in "overview" mode; used in editors, etc.
      actionOverviewMoveForward,
      actionOverviewMoveLeft,
      actionOverviewMoveRight,
      actionOverviewMoveBackward,

      // selection
      actionSelectNext,    ///< selects next mobile in range
      actionSelectPrev,    ///< selects previous mobile in range

      // actions carried out by player
      actionAction1,
      actionAction2,
      actionAction3,
      actionAction4,
   };

   /// callback for keyboard action; bool indicates if key was pressed or depressed
   typedef std::function<void(bool)> T_fnOnKeyboardAction;

   /// callback for keyboard action; key-down-only handler
   typedef std::function<void()> T_fnOnKeyboardActionKeyEvent;

   /// ctor
   KeyboardActionManager() throw()
      :m_uiNextHandlerId(0)
   {}

   /// registers a key by description for a given action
   void RegisterActionKey(const CString& cszKeyDescription, T_enAction enAction);

   /// registers a handler for a given action
   KeyActionHandlePtr RegisterActionHandler(T_enAction enAction, T_fnOnKeyboardAction fnOnKeyboardAction);

   /// registers a key-down-only handler for a given action
   KeyActionHandlePtr RegisterActionHandler(T_enAction enAction, bool bOnPressed, T_fnOnKeyboardActionKeyEvent fnOnKeyboardActionKeyEvent);

   /// called to process a keyboard event
   void OnKeyboardEvent(bool bKeyDown, unsigned int uiSym, unsigned int uiMod);

   /// function usable in std::bind to convert to key-down-only action function type
   static void WrapOnlyKeyPressed(bool bPressed, T_fnOnKeyboardActionKeyEvent fn)
   {
      if (bPressed)
         fn();
   }

   /// function usable in std::bind to convert to key-down- or key-up-only action function type
   static void WrapOnlyKeyEvent(bool bPressed, bool bExpectedKeyEvent, T_fnOnKeyboardActionKeyEvent fn)
   {
      if (bPressed == bExpectedKeyEvent)
         fn();
   }

private:
   friend class KeyActionHandle;

   /// parses key description and returns key/sym value
   unsigned int ParseKeyDescription(const CString& cszKeyDescription);

   /// unregisters handler
   void UnregisterActionHandler(T_enAction enAction, unsigned int uiHandlerId);

private:
   /// mapping of key scancodes to key actions
   typedef std::map<unsigned int, T_enAction> T_mapAllKeyActions;

   /// action handler entry
   struct ActionHandlerEntry
   {
      /// ctor
      ActionHandlerEntry(T_enAction enAction, unsigned int uiHandlerId, T_fnOnKeyboardAction fnOnKeyboardAction)
         :m_enAction(enAction),
          m_uiHandlerId(uiHandlerId),
          m_fnOnKeyboardAction(fnOnKeyboardAction)
      {
      }

      /// action
      T_enAction m_enAction;

      /// handler id
      unsigned int m_uiHandlerId;

      /// assigned keyboard action
      T_fnOnKeyboardAction m_fnOnKeyboardAction;

      /// less operator
      bool operator<(const ActionHandlerEntry& rhs) const throw()
      {
         return m_enAction < rhs.m_enAction;
      }
   };

   /// tyoe for action handler map
   typedef std::multiset<ActionHandlerEntry> T_mapAllActionHandler;

   /// map from all keys to actions; only one action per key possible
   T_mapAllKeyActions m_mapAllKeyActions;

   /// map from all actions to all handler; multiple handler possible
   T_mapAllActionHandler m_mapAllActionHandler;

   /// next handler id
   unsigned int m_uiNextHandlerId;
};
