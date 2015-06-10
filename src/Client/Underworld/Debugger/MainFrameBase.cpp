//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file MainFrameBase.cpp Main application frame base class
//

// includes

// includes
#include "stdafx.h"
#include "MainFrameBase.hpp"
#include "DockingWindowBase.hpp"
#include "AboutDlg.hpp"

LRESULT MainFrameBase::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);

   SetupCmdbar();
   //SetupToolbar();

   CreateSimpleStatusBar();
   UISetCheck(ID_VIEW_STATUS_BAR, 1);

   SetupMDIClient();

   // register object for message filtering and idle updates
   CMessageLoop* pLoop = _Module.GetMessageLoop();
   ATLASSERT(pLoop != NULL);
   pLoop->AddMessageFilter(this);
   pLoop->AddIdleHandler(this);

   // floating stuff
   InitializeDockingFrame();

   UpdateLayout();

   return 0;
}

BOOL MainFrameBase::PreTranslateMessage(MSG* pMsg)
{
   if (pMsg == NULL)
      return FALSE;

   bool bCalledBaseClass = false;

   if (pMsg->hwnd == m_hWnd || pMsg->hwnd == m_hWndMDIClient)
   {
      // Message is sent directly to main frame or
      // to the MDIClient window
      if (baseClass::PreTranslateMessage(pMsg))
         return TRUE;

      bCalledBaseClass = true;
   }

   HWND hWndFocus = ::GetFocus();
   HWND hWndMDIActive = MDIGetActive();

   if ((hWndMDIActive == hWndFocus) || (::IsChild(hWndMDIActive, hWndFocus)))
   {
      // Message is sent to Active MDI child frame
      // or a descendant
      // NOTE: IsChild checks if the window is a direct child or a descendant

      if (baseClass::PreTranslateMessage(pMsg))
         return TRUE;

      bCalledBaseClass = true;

      if (hWndMDIActive != NULL)
      {
         return (BOOL)::SendMessage(hWndMDIActive, WM_FORWARDMSG, 0, (LPARAM)pMsg);
      }
   }

   if (!bCalledBaseClass)
   {
      // If the base class hasn't already had a shot at doing
      // PreTranslateMessage (because the main frame or an
      // MDI child didn't have focus), call it now
      if (baseClass::PreTranslateMessage(pMsg))
         return TRUE;

      // Give active MDI child a chance.
      if (hWndMDIActive != NULL)
      {
         return (BOOL)::SendMessage(hWndMDIActive, WM_FORWARDMSG, 0, (LPARAM)pMsg);
      }
   }

   return FALSE;
}

BOOL MainFrameBase::OnIdle()
{
   UIUpdateToolBar();

   return FALSE;
}

LRESULT MainFrameBase::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   PostMessage(WM_CLOSE);
   return 0;
}

LRESULT MainFrameBase::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   static BOOL bVisible = TRUE;   // initially visible
   bVisible = !bVisible;

   CReBarCtrl rebar = m_hWndToolBar;
   int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);   // toolbar is 2nd added band
   rebar.ShowBand(nBandIndex, bVisible);

   UISetCheck(ID_VIEW_TOOLBAR, bVisible);
   UpdateLayout();
   return 0;
}

LRESULT MainFrameBase::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
   ::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
   UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
   UpdateLayout();
   return 0;
}

LRESULT MainFrameBase::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   AboutDlg dlg;
   dlg.DoModal();
   return 0;
}

bool MainFrameBase::ShowHideDockingWindow(DockingWindowBase& dockingWindow)
{
   // determine if docking window is visible
   bool bVisible = dockingWindow.IsWindow() && dockingWindow.IsWindowVisible() &&
      (dockingWindow.IsDocking() || dockingWindow.IsFloating());

   if (bVisible)
   {
      // when docking, undock window, else hide window
      if (dockingWindow.IsDocking())
         dockingWindow.Undock();
      else
         dockingWindow.Hide();
      SetFocus();
   }
   else
   {
      if (!dockingWindow.IsWindow())
      {
         CRect rect(CPoint(0, 0), dockingWindow.GetFloatingSize());

         DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
         dockingWindow.Create(m_hWnd, rect, dockingWindow.GetDockWindowCaption(), dwStyle);
      }

      DockDockingWindow(dockingWindow);
   }

   return !bVisible;
}

void MainFrameBase::DockDockingWindow(DockingWindowBase& dockingWindow)
{
   CSize dockSize = dockingWindow.GetDockingSize();
   dockwins::CDockingSide dockSide = dockingWindow.GetPreferredDockingSide();

   // TODO search proper bar to dock to
   int nBar = 0;

   DockWindow(dockingWindow, dockSide,
      nBar, float(0.0)/*fPctPos*/, dockSize.cx, dockSize.cy);
}

void MainFrameBase::SetupCmdbar()
{
   HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);

   m_CmdBar.AttachMenu(GetMenu());
   m_CmdBar.LoadImages(IDR_MAINFRAME);

   SetMenu(NULL);
   AddSimpleReBarBand(hWndCmdBar);
}

void MainFrameBase::SetupToolbar()
{
   HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

   AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

   UIAddToolBar(hWndToolBar);
   UISetCheck(ID_VIEW_TOOLBAR, 1);
}

void MainFrameBase::SetupMDIClient()
{
   m_tabbedChildWindow.SetReflectNotifications(true);

   m_tabbedChildWindow.SetTabStyles(CTCS_TOOLTIPS | CTCS_SCROLL);
   m_tabbedChildWindow.Create(m_hWnd, rcDefault);
   m_tabbedChildWindow.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

   CreateMDIClient();

   // subclass mdi client
   m_tabbedClient.SetTabOwnerParent(m_hWnd);
   ATLVERIFY(TRUE == m_tabbedClient.SubclassWindow(m_hWndMDIClient));

   m_CmdBar.SetMDIClient(m_hWndMDIClient);
}
