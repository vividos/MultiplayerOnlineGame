//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file GameStringsView.hpp Game strings view
//
#pragma once

// includes
#include "Resource.h"
#include "DockingWindowBase.hpp"
#include "ChildWindowBase.hpp"

/// game strings view form
class GameStringsView:
   public CDialogImpl<GameStringsView>,
   public CDialogResize<GameStringsView>,
   public CWinDataExchange<GameStringsView>
{
public:
   /// ctor
   GameStringsView(DebugClient& debugClient)
      :m_debugClient(debugClient)
   {
   }
   /// dtor
   virtual ~GameStringsView();

   enum { IDD = IDD_GAMESTRINGS };

   BOOL PreTranslateMessage(MSG* pMsg)
   {
      return IsDialogMessage(pMsg);
   }

private:
   BEGIN_MSG_MAP(GameStringsView)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      COMMAND_CODE_HANDLER(CBN_SELCHANGE, OnComboSelChange);
      CHAIN_MSG_MAP(CDialogResize<GameStringsView>)
   END_MSG_MAP()

   BEGIN_DLGRESIZE_MAP(GameStringsView)
      DLGRESIZE_CONTROL(IDC_COMBO_TEXTBLOCK, DLSZ_SIZE_X)
      DLGRESIZE_CONTROL(IDC_LIST_STRINGS, DLSZ_SIZE_X | DLSZ_SIZE_Y)
   END_DLGRESIZE_MAP()

   BEGIN_DDX_MAP(GameStringsView)
      DDX_CONTROL_HANDLE(IDC_LIST_STRINGS, m_listStrings)
      DDX_CONTROL_HANDLE(IDC_COMBO_TEXTBLOCK, m_comboBlocks)
   END_DDX_MAP()

   LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
   LRESULT OnComboSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

   /// initializes combobox content
   void InitCombobox();

   /// refreshes string list for current block
   void RefreshStringList(Uint16 uiBlock);

private:
   /// debug client
   DebugClient& m_debugClient;

   /// list view with all strings
   CListViewCtrl m_listStrings;

   /// combobox to select string blocks
   CComboBox m_comboBlocks;
};

/// child frame for game strings view
class GameStringsViewChildFrame : public ChildWindowBase<GameStringsView, IDR_GAME_STRINGS>
{
public:
   /// ctor
   GameStringsViewChildFrame(DebugClient& debugClient)
   {
      m_upView.reset(new GameStringsView(debugClient));

      m_bDynamicWindow = true;
   }
};
