//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file DockingWindowBase.hpp Docking window base class
//
#pragma once

/// base class for child windows with tabbing support
/// \tparam TView type of view to host in this child window
/// \tparam IDR_WINDOW window id of child window
template <typename TView, unsigned int IDR_WINDOW>
class ChildWindowBase :
   public CTabbedMDIChildWindowImpl<ChildWindowBase<TView, IDR_WINDOW>>
{
   typedef ChildWindowBase<TView, IDR_WINDOW> thisClass;
   typedef CTabbedMDIChildWindowImpl<ChildWindowBase<TView, IDR_WINDOW>> baseClass;

public:
   /// ctor
   ChildWindowBase()
      :m_bDynamicWindow(false)
   {
   }

   DECLARE_FRAME_WND_CLASS(NULL, IDR_WINDOW)

protected:
   // message map
   BEGIN_MSG_MAP(thisClass)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)
      CHAIN_MSG_MAP(baseClass)
   END_MSG_MAP()

   /// called on last message
   virtual void OnFinalMessage(HWND /*hWnd*/)
   {
      if (m_bDynamicWindow)
         delete this;
   }

   /// called when window is created
   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
   {
      ATLASSERT(m_upView != nullptr); // create m_upView in ctor!

      m_hWndClient = m_upView->Create(m_hWnd, rcDefault, NULL);

      bHandled = FALSE;
      return 1;
   }

   /// called on forwarded message
   LRESULT OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
   {
      LPMSG pMsg = (LPMSG)lParam;

      if (baseClass::PreTranslateMessage(pMsg))
         return TRUE;

      if (m_upView != nullptr)
         return m_upView->PreTranslateMessage(pMsg);

      return FALSE;
   }

protected:
   /// indicates if window is dynamically created and should be destroyed on final message
   bool m_bDynamicWindow;

   /// hosted view
   std::unique_ptr<TView> m_upView;
};
