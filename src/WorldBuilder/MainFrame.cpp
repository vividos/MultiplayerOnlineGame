//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file WorldBuilder/MainFrame.cpp Main application frame
//

// includes
#include "stdafx.h"
#include "MainFrame.hpp"
#include "WorldGenerator.hpp"
#include "WorldRenderManager.hpp"
#include "WorldBuilderController.hpp"
#include "PerspectiveCamera.hpp"

/// settings registry key (subkey "Ribbon" is used for menu band)
LPCTSTR c_pszSettingsRegkey = _T("Software\\MultiplayerOnlineGame\\WorldBuilder");

/// render cycle time, in ms; 50 ms gives about 20 fps
const unsigned int c_uiRenderCycleInMs = 50;

/// ctor
MainFrame::MainFrame() throw()
:m_upWorldGenerator(new WorldGenerator(1024)),
 m_ioServiceGenerator(true, _T("WorldBuilder worker")) // thread with default work
{
   m_upWorldGenerator->SetUpdateCallback(
      std::bind(&MainFrame::OnUpdateWorldGenerator, this, std::placeholders::_1, std::placeholders::_2));

   // start worker thread
   m_ioServiceGenerator.Run();
}

MainFrame::~MainFrame() throw()
{
}

BOOL MainFrame::PreTranslateMessage(MSG* pMsg)
{
   if (m_upRenderView != nullptr && m_upRenderView->PreTranslateMessage(pMsg))
      return TRUE;

   return BaseClass::PreTranslateMessage(pMsg);
}

BOOL MainFrame::OnIdle()
{
   UIUpdateToolBar();

   if (m_upController != nullptr && m_upRenderView != nullptr)
   {
      m_upRenderView->Activate();
      m_upController->OnIdle();
      m_upRenderView->Deactivate();
   }

   if (m_upRenderView != nullptr)
      m_upRenderView->RedrawWindow();

   return FALSE;
}

LRESULT MainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   // check if ribbon is available
   bool bRibbonUI = RunTimeHelper::IsRibbonUIAvailable();

   if (!bRibbonUI)
   {
      // TODO add message box
      return 0;
   }

   CRibbonPersist(c_pszSettingsRegkey).Restore(bRibbonUI, m_hgRibbonSettings);

   SetupStatusBar();

   CreateView();

   // register object for message filtering and idle updates
   CMessageLoop* pLoop = _Module.GetMessageLoop();
   ATLASSERT(pLoop != nullptr);
   pLoop->AddMessageFilter(this);
   pLoop->AddIdleHandler(this);

   ShowRibbonUI(true);

   SetTimer(IDT_RENDER, c_uiRenderCycleInMs);

   return 0;
}

LRESULT MainFrame::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
   bHandled = false; // let app process message

   if (RunTimeHelper::IsRibbonUIAvailable())
   {
      bool bRibbonUI = IsRibbonUI();
      if (bRibbonUI)
         SaveRibbonSettings();

      CRibbonPersist(c_pszSettingsRegkey).Save(bRibbonUI, m_hgRibbonSettings);
   }

   return 0;
}

LRESULT MainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
   KillTimer(IDT_RENDER);

   m_upRenderView->DestroyWindow();
   m_upRenderView.reset();

   // unregister message filtering and idle updates
   CMessageLoop* pLoop = _Module.GetMessageLoop();
   ATLASSERT(pLoop != nullptr);
   pLoop->RemoveMessageFilter(this);
   pLoop->RemoveIdleHandler(this);

   bHandled = false;
   return 1;
}

LRESULT MainFrame::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
   bHandled = false;

   if (m_progressBar.IsWindow())
   {
      CRect rcPane;
      m_statusBar.GetPaneRect(IDR_PANE_PROGRESS, &rcPane);

      m_progressBar.MoveWindow(&rcPane, true);
   }

   return 0;
}

LRESULT MainFrame::OnMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
   bHandled = false;

   if (m_progressBar.IsWindow())
   {
      CRect rcPane;
      m_statusBar.GetPaneRect(IDR_PANE_PROGRESS, &rcPane);

      m_progressBar.MoveWindow(&rcPane, true);
   }

   return 0;
}

LRESULT MainFrame::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   if (wParam == IDT_RENDER && m_upRenderView != nullptr)
      m_upRenderView->RedrawWindow();

   return 0;
}

LRESULT MainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   PostMessage(WM_CLOSE);
   return 0;
}

LRESULT MainFrame::OnWorldCreate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   m_ioServiceGenerator.Get().post(
      std::bind(&MainFrame::GenerateWorld, this));

   return 0;
}

void MainFrame::CreateView()
{
   m_upRenderView.reset(new RenderView);
   m_hWndClient = m_upRenderView->Create(m_hWnd, rcDefault);

   std::shared_ptr<RenderEngine> spRenderEngine = m_upRenderView->GetRenderEngine();

   m_spWorldRenderManager.reset(new WorldRenderManager(*spRenderEngine, *m_upWorldGenerator.get()));

   m_spWorldRenderManager->GetCamera()->SetPosition(
      Vector3d(390.0, 440.0, 1140.0), -349.0, -42.0);

   m_upController.reset(new WorldBuilderController(m_spWorldRenderManager->GetCamera()));

   spRenderEngine->SetScenegraph(m_spWorldRenderManager);
}

/// \see http://www.codeproject.com/Articles/2948/How-to-use-the-WTL-multipane-status-bar-control
void MainFrame::SetupStatusBar()
{
   // added WS_CLIPCHILDREN to default styles
   CreateSimpleStatusBar(
      ATL_IDS_IDLEMESSAGE, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | SBARS_SIZEGRIP | WS_CLIPCHILDREN);

   m_statusBar.SubclassWindow(m_hWndStatusBar);

   // set status bar panes. ID_DEFAULT_PANE is defined by WTL
   int arrPanes[] = { ID_DEFAULT_PANE, IDR_PANE_PROGRESS };

   m_statusBar.SetPanes(arrPanes, sizeof(arrPanes) / sizeof(int), false);

   // set status bar pane widths using local workaround
   int arrWidths[] = { 0, 150 };
   SetPaneWidths(arrWidths, sizeof(arrWidths) / sizeof(int));
}

void MainFrame::SetStatusText(const CString& cszText)
{
   ::SetWindowText(m_hWndStatusBar, cszText);
}

void MainFrame::SetPaneWidths(int* arrWidths, int nPanes)
{ 
   // find the size of the borders
   int arrBorders[3];
   m_statusBar.GetBorders(arrBorders);

   // calculate right edge of default pane (0)
   arrWidths[0] += arrBorders[2];
   for (int i = 1; i < nPanes; i++)
      arrWidths[0] += arrWidths[i];

   // calculate right edge of remaining panes (1 thru nPanes-1)
   for (int j = 1; j < nPanes; j++)
      arrWidths[j] += arrBorders[2] + arrWidths[j - 1];

   // set the pane widths
   m_statusBar.SetParts(m_statusBar.m_nPanes, arrWidths); 
}

void MainFrame::GenerateWorld()
{
   m_upWorldGenerator->Generate(WorldGenerator::T_enIslandShape::islandPerlinNoise, 42);
}

void MainFrame::OnUpdateWorldGenerator(const CString& cszStatus, bool bUpdatedGraph)
{
   // TODO send message back to window
   if (bUpdatedGraph)
   {
      m_spWorldRenderManager->SetWorldViewMode(WorldRenderManager::worldViewPolygonGraph);
   }

   SetStatusText(cszStatus);
}
