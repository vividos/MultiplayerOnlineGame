//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file MainFrameBase.hpp Main application frame base class
//
#pragma once

// includes
#include "DebugClient.hpp"
#include "Resource.h"

// forward references
class DockingWindowBase;

/// main frame base class
class MainFrameBase :
   public dockwins::CMDIDockingFrameImpl<MainFrameBase>,
   public CUpdateUI<MainFrameBase>,
   public CMessageFilter,
   public CIdleHandler
{
   typedef dockwins::CMDIDockingFrameImpl<MainFrameBase> baseClass;

public:
   DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

public:
   /// called for every message, before processing it
   virtual BOOL PreTranslateMessage(MSG* pMsg);

   /// called for idle processing
   virtual BOOL OnIdle();

protected:
   /// shows or hides docking windows
   bool ShowHideDockingWindow(DockingWindowBase& dockingWindow);

   /// docks an undocked docking window
   void DockDockingWindow(DockingWindowBase& dockingWindow);

private:
   // message map
   BEGIN_MSG_MAP(MainFrameBase)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
      COMMAND_ID_HANDLER(ID_WINDOW_CASCADE, OnWindowCascade)
      COMMAND_ID_HANDLER(ID_WINDOW_TILE_HORZ, OnWindowTileHorizontal)
      COMMAND_ID_HANDLER(ID_WINDOW_TILE_VERT, OnWindowTileVertical)
      COMMAND_ID_HANDLER(ID_WINDOW_ARRANGE, OnWindowArrangeIcons)
      CHAIN_MDI_CHILD_COMMANDS()
      CHAIN_MSG_MAP(CUpdateUI<MainFrameBase>)
      CHAIN_MSG_MAP(baseClass)
      REFLECT_NOTIFICATIONS()
   END_MSG_MAP()

   // update map for menus and toolbars
   BEGIN_UPDATE_UI_MAP(MainFrameBase)
      UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
      UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
   END_UPDATE_UI_MAP()

// Handler prototypes (uncomment arguments if needed):
//   LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//   LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//   LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

   // message handler

   LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
   LRESULT OnFileExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
   LRESULT OnViewToolBar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
   LRESULT OnViewStatusBar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
   LRESULT OnAppAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

   LRESULT OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      MDICascade();
      return 0;
   }

   LRESULT OnWindowTileHorizontal(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      MDITile(MDITILE_HORIZONTAL);
      return 0;
   }

   LRESULT OnWindowTileVertical(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      MDITile(MDITILE_VERTICAL);
      return 0;
   }

   LRESULT OnWindowArrangeIcons(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
   {
      MDIIconArrange();
      return 0;
   }

   /// sets up command bar
   void SetupCmdbar();

   /// sets up toolbar
   void SetupToolbar();

   /// sets up MDI client window
   void SetupMDIClient();

private:
   /// MDI command bar for tabbing, with XP look
   CTabbedMDICommandBarCtrlXP m_CmdBar;

   /// tabbed MDI client window
   CTabbedMDIClient<CDotNetTabCtrl<CTabViewTabItem>> m_tabbedClient;

   /// tabbed child window
   CTabbedChildWindow<CDotNetTabCtrl<CTabViewTabItem>> m_tabbedChildWindow;
};
