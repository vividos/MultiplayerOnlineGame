//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file KeyboardActionManager.cpp Keyboard action manager
//

// includes
#include "StdAfx.h"
#include "KeyboardActionManager.hpp"
#include <SDL_keycode.h>

//
// KeyActionHandle
//

/// handle for registered key action
class KeyActionHandle: public boost::noncopyable
{
public:
   /// ctor
   KeyActionHandle(KeyboardActionManager& keyboardActionManager,
      KeyboardActionManager::T_enAction enAction, unsigned int uiHandlerId)
      :m_keyboardActionManager(keyboardActionManager),
       m_enAction(enAction),
       m_uiHandlerId(uiHandlerId)
   {
   }

   /// dtor
   ~KeyActionHandle()
   {
      m_keyboardActionManager.UnregisterActionHandler(m_enAction, m_uiHandlerId);
   }

private:
   /// ref to keyboard action manager
   KeyboardActionManager& m_keyboardActionManager;

   /// registered action
   KeyboardActionManager::T_enAction m_enAction;

   /// handler id
   unsigned int m_uiHandlerId;
};

//
// KeyboardActionManager
//

void KeyboardActionManager::RegisterActionKey(const CString& cszKeyDescription, T_enAction enAction)
{
   // parse description
   uint64_t uiKeySymValue = ParseKeyDescription(cszKeyDescription);
   if (uiKeySymValue != 0)
      m_mapAllKeyActions[uiKeySymValue] = enAction;
}

KeyActionHandlePtr KeyboardActionManager::RegisterActionHandler(T_enAction enAction, T_fnOnKeyboardAction fnOnKeyboardAction)
{
   unsigned int uiHandlerId = m_uiNextHandlerId++;
   m_mapAllActionHandler.insert(ActionHandlerEntry(enAction, uiHandlerId, fnOnKeyboardAction));

   return KeyActionHandlePtr(new KeyActionHandle(*this, enAction, uiHandlerId));
}

KeyActionHandlePtr KeyboardActionManager::RegisterActionHandler(T_enAction enAction, bool bOnPressed, T_fnOnKeyboardActionKeyEvent fnOnKeyboardActionKeyEvent)
{
   return KeyboardActionManager::RegisterActionHandler(enAction,
      std::bind(&KeyboardActionManager::WrapOnlyKeyEvent, std::placeholders::_1, bOnPressed, fnOnKeyboardActionKeyEvent));
}

bool KeyboardActionManager::OnKeyboardEvent(bool bKeyDown, unsigned int sym, unsigned int mod)
{
   // remove num, caps and mode
   mod &= ~(KMOD_NUM | KMOD_CAPS | KMOD_MODE);

   // set left alt when right alt was set and vice versa
   if ((mod & KMOD_ALT) != 0)
      mod |= KMOD_ALT;

   // same for shift
   if ((mod & KMOD_SHIFT) != 0)
      mod |= KMOD_SHIFT;

   // same for ctrl
   if ((mod & KMOD_CTRL) != 0)
      mod |= KMOD_CTRL;

   uint64_t uiKeySymValue = (static_cast<uint64_t>(mod) << 32) | static_cast<uint64_t>(sym);

   // find action
   T_mapAllKeyActions::const_iterator iter = m_mapAllKeyActions.find(uiKeySymValue);
   if (iter == m_mapAllKeyActions.end())
      return false; // no action defined

   T_enAction enAction = iter->second;

   // call all handler
   // note: since find() wants an ActionHandlerEntry, we create one with default params for the
   // parameters not relevant to the search
   T_mapAllActionHandler::const_iterator iterHandler =
      m_mapAllActionHandler.find(ActionHandlerEntry(enAction, 0, T_fnOnKeyboardAction()));

   while (iterHandler != m_mapAllActionHandler.end() && iterHandler->m_enAction == enAction)
   {
      iterHandler->m_fnOnKeyboardAction(bKeyDown);
      ++iterHandler;
   }

   return true;
}

/// known modifier keys
struct ModifierKeyNameMap
{
   LPCTSTR pszName;     ///< modifier name
   unsigned int uiMod;  ///< modifier value
};

/// modifier key name to value map
ModifierKeyNameMap g_aModifierKeys[] =
{
   { _T("strg"),  KMOD_CTRL },
   { _T("ctrl"),  KMOD_CTRL },
   { _T("shift"), KMOD_SHIFT },
   { _T("alt"),   KMOD_ALT },
};

/// known keys
struct KeyNameMap
{
   LPCTSTR pszName;  ///< key name
   SDL_Keycode enKey;     ///< key value
};

/// key name to key map
KeyNameMap g_aKeys[] =
{
   { _T("backspace"),  SDLK_BACKSPACE },
   { _T("tab"),  SDLK_TAB },
   { _T("clear"),  SDLK_CLEAR },
   { _T("enter"),  SDLK_RETURN },
   { _T("return"),  SDLK_RETURN },
   { _T("pause"),  SDLK_PAUSE },
   { _T("esc"),  SDLK_ESCAPE },
   { _T("space"),  SDLK_SPACE },
   { _T("!"),  SDLK_EXCLAIM },
   { _T("\""),  SDLK_QUOTEDBL },
   { _T("#"),  SDLK_HASH },
   { _T("$"),  SDLK_DOLLAR },
   { _T("&"),  SDLK_AMPERSAND },
   { _T("'"),  SDLK_QUOTE },
   { _T("("),  SDLK_LEFTPAREN },
   { _T(")"),  SDLK_RIGHTPAREN },
   { _T("*"),  SDLK_ASTERISK },
   { _T("+"),  SDLK_PLUS },
   { _T(","),  SDLK_COMMA },
   { _T("-"),  SDLK_MINUS },
   { _T("-"),  SDLK_PERIOD },
   { _T("/"),  SDLK_SLASH },

   { _T("."),  SDLK_COLON },
   { _T(";"),  SDLK_SEMICOLON },
   { _T("<"),  SDLK_LESS },
   { _T("="),  SDLK_EQUALS },
   { _T(">"),  SDLK_GREATER },
   { _T("?"),  SDLK_QUESTION },
   { _T("@"),  SDLK_AT },

   { _T("["),  SDLK_LEFTBRACKET },
   { _T("\\"),  SDLK_BACKSLASH },
   { _T("]"),  SDLK_RIGHTBRACKET },
   { _T("^"),  SDLK_CARET },
   { _T("_"),  SDLK_UNDERSCORE },
   { _T("backquote"),  SDLK_BACKQUOTE },
   { _T("del"),  SDLK_DELETE },

   { _T("numlock"), SDLK_NUMLOCKCLEAR },
   { _T("num0"),  SDLK_KP_0 },
   { _T("num1"),  SDLK_KP_1 },
   { _T("num2"),  SDLK_KP_2 },
   { _T("num3"),  SDLK_KP_3 },
   { _T("num4"),  SDLK_KP_4 },
   { _T("num5"),  SDLK_KP_5 },
   { _T("num6"),  SDLK_KP_6 },
   { _T("num7"),  SDLK_KP_7 },
   { _T("num8"),  SDLK_KP_8 },
   { _T("num9"),  SDLK_KP_9 },
   { _T("numperiod"),  SDLK_KP_PERIOD },
   { _T("numdivide"),  SDLK_KP_DIVIDE },
   { _T("nummultiply"),  SDLK_KP_MULTIPLY },
   { _T("numminus"),  SDLK_KP_MINUS },
   { _T("numplus"),  SDLK_KP_PLUS },
   { _T("numenter"),  SDLK_KP_ENTER },
   { _T("numequals"),  SDLK_KP_EQUALS },

   { _T("up"),  SDLK_UP },
   { _T("down"),  SDLK_DOWN },
   { _T("right"),  SDLK_RIGHT },
   { _T("left"),  SDLK_LEFT },
   { _T("ins"),  SDLK_INSERT },
   { _T("home"),  SDLK_HOME },
   { _T("end"),  SDLK_END },
   { _T("pgup"),  SDLK_PAGEUP },
   { _T("pgdn"),  SDLK_PAGEDOWN },
   { _T("f1"),  SDLK_F1 },
   { _T("f2"),  SDLK_F2 },
   { _T("f3"),  SDLK_F3 },
   { _T("f4"),  SDLK_F4 },
   { _T("f5"),  SDLK_F5 },
   { _T("f6"),  SDLK_F6 },
   { _T("f7"),  SDLK_F7 },
   { _T("f8"),  SDLK_F8 },
   { _T("f9"),  SDLK_F9 },
   { _T("f10"),  SDLK_F10 },
   { _T("f11"),  SDLK_F11 },
   { _T("f12"),  SDLK_F12 },
   { _T("f13"),  SDLK_F13 },
   { _T("f14"),  SDLK_F14 },
   { _T("f15"),  SDLK_F15 },

   { _T("0"),  SDLK_0 },
   { _T("1"),  SDLK_1 },
   { _T("2"),  SDLK_2 },
   { _T("3"),  SDLK_3 },
   { _T("4"),  SDLK_4 },
   { _T("5"),  SDLK_5 },
   { _T("6"),  SDLK_6 },
   { _T("7"),  SDLK_7 },
   { _T("8"),  SDLK_8 },
   { _T("9"),  SDLK_9 },
};


uint64_t KeyboardActionManager::ParseKeyDescription(const CString& cszKeyDescription)
{
   CString cszModifierKey, cszKey(cszKeyDescription);
   int iPos = cszKeyDescription.Find(_T('+'));
   if (iPos != -1)
   {
      cszModifierKey = cszKeyDescription.Left(iPos);
      cszKey = cszKeyDescription.Mid(iPos+1);
   }

   uint64_t uiKeySymValue = 0;

   // search for modifier key in mapping
   if (!cszModifierKey.IsEmpty())
   {
      cszModifierKey.MakeLower();
      for (size_t i=0, iMax=sizeof(g_aModifierKeys)/sizeof(*g_aModifierKeys); i<iMax; i++)
         if (cszModifierKey == g_aModifierKeys[i].pszName)
         {
            uiKeySymValue |= (static_cast<uint64_t>(g_aModifierKeys[i].uiMod) << 32);
            break;
         }
   }

   // search for key
   bool bFoundKey = false;
   cszKey.MakeLower();
   for (size_t i=0, iMax=sizeof(g_aKeys)/sizeof(*g_aKeys); i<iMax; i++)
   {
      if (cszKey == g_aKeys[i].pszName)
      {
         uiKeySymValue |= g_aKeys[i].enKey;
         bFoundKey = true;
         break;
      }
   }

   if (!bFoundKey && cszKey.GetLength() == 1)
   {
      unsigned int uiKey = SDLK_UNKNOWN;

      // may be a single ascii key; convert that one
      TCHAR ch = cszKey[0];
      if (ch >= _T('0') && ch <= _T('9'))
      {
         uiKey = ch - _T('0') + SDLK_0;
      }
      else
      if (ch >= _T('a') && ch <= _T('z'))
      {
         uiKey = ch - _T('a') + SDLK_a;
      }

      if (uiKey != SDLK_UNKNOWN)
      {
         uiKeySymValue |= uiKey;
         bFoundKey = true;
      }
   }

   return bFoundKey ? uiKeySymValue : 0;
}

void KeyboardActionManager::UnregisterActionHandler(T_enAction enAction, unsigned int uiHandlerId)
{
   // remove correct handler by using action and handler id
   T_mapAllActionHandler::iterator iter =
      m_mapAllActionHandler.find(ActionHandlerEntry(enAction, uiHandlerId, T_fnOnKeyboardAction()));

   while (iter != m_mapAllActionHandler.end() && iter->m_enAction == enAction)
   {
      if (iter->m_uiHandlerId == uiHandlerId)
      {
         m_mapAllActionHandler.erase(iter);
         return;
      }
      ++iter;
   }
   ATLASSERT(false);
}
