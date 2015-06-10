//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Debugger/stdafx.cpp Precompiled header support
//

// includes
#include "stdafx.h"

#include <dockimpl.cpp>

/// \brief checks if parent window reflects notifications
/// \details The check is done by sending a WM_NOTIFY message with notification code
/// 0xFFFF (which surely isn't used by the control) to the parent class and
/// checking if the notification is reflected through an OCM_NOTIFY message to
/// us. If not, the user forgot to add the macro.
void CheckAddedReflectNotifications(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   static bool m_fInCheck = false;
   static bool m_fCheckPassed = false;

   if (!m_fCheckPassed)
   {
      if (m_fInCheck)
      {
         // did we get the proper notification message?
         if (uMsg == OCM_NOTIFY && wParam == (WPARAM)-1 && ((LPNMHDR)lParam)->code == (UINT)-1)
         {
            m_fInCheck = false;
            m_fCheckPassed = true;
         }
      }
      else
      {
         // start check
         m_fInCheck = true;

         // prepare notification
         NMHDR nmhdr;
         nmhdr.code = static_cast<UINT>(-1);
         nmhdr.hwndFrom = hWnd;
         nmhdr.idFrom = static_cast<UINT_PTR>(-1);
         ::SendMessage(::GetParent(hWnd), WM_NOTIFY, (WPARAM)-1, (LPARAM)&nmhdr);

         ATLASSERT(m_fCheckPassed == true); // Warning! forgot to add REFLECT_NOTIFICATIONS() to base class!
         m_fInCheck = false;
      }
   }
}
