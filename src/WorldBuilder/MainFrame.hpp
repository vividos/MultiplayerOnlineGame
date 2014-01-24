//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file WorldBuilder/MainFrame.hpp Main application frame
//
#pragma once

// includes
#include "resource.h"
#include "res/MainFrameRibbon.h"
#include "RenderView.hpp"
#include "IoServiceThread.hpp"

// forward references
class WorldGenerator;
class WorldRenderManager;

/// \brief application main frame
/// \details uses ribbon for commands
/// \see http://www.codeproject.com/Articles/54116/Relook-your-Old-and-New-Native-Applications-with-a
class MainFrame :
   public CRibbonFrameWindowImpl<MainFrame>,
   public CMessageFilter,
   public CIdleHandler
{
   /// base class typedef
   typedef CRibbonFrameWindowImpl<MainFrame> BaseClass;

public:
   /// ctor
   MainFrame() throw();
   /// dtor
   virtual ~MainFrame() throw();

   DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

   /// sets status bar text
   void SetStatusText(const CString& cszText);

private:
   friend CDynamicUpdateUI;
   friend CRibbonFrameWindowImplBase;

   /// pretranslates message
   virtual BOOL PreTranslateMessage(MSG* pMsg);

   /// called when message loop is idle
   virtual BOOL OnIdle();

   BEGIN_UPDATE_UI_MAP(MainFrame)
   END_UPDATE_UI_MAP()

   BEGIN_MSG_MAP(MainFrame)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_CLOSE, OnClose)
      MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      MESSAGE_HANDLER(WM_MOVE, OnMove)
      COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
      COMMAND_ID_HANDLER(ID_WORLD_CREATE, OnWorldCreate)
      CHAIN_MSG_MAP(BaseClass)
   END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
// LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
// LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
// LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
   LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
   LRESULT OnWorldCreate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
   /// sets up toolbar
   void SetupToolbar(HWND hWndCmdBar);

   /// sets up status bar
   void SetupStatusBar();

   /// sets status bar pane widths
   void SetPaneWidths(int* arrWidths, int nPanes);

   /// generates world; called in worker thread
   void GenerateWorld();

   /// called when world generator has an updated status and/or graph
   void OnUpdateWorldGenerator(const CString& cszStatus, bool bUpdatedGraph);

private:
   // UI

   /// render view
   std::unique_ptr<RenderView> m_upRenderView;

   /// status bar
   CMultiPaneStatusBarCtrl m_statusBar;

   /// progress bar
   CProgressBarCtrl m_progressBar;

   /// world render manager
   std::shared_ptr<WorldRenderManager> m_spWorldRenderManager;

   // model

   /// world generator
   std::unique_ptr<WorldGenerator> m_upWorldGenerator;

   /// io service thread for world generator
   IoServiceThread m_ioServiceGenerator;
};
