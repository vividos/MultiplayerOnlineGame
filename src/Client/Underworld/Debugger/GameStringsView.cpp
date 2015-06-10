//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file GameStringsView.hpp Game strings view
//
#pragma once

// includes
#include "stdafx.h"
#include "GameStringsView.hpp"
#include "DebugClient.hpp"

// methods

GameStringsView::~GameStringsView()
{
   m_listStrings.Detach();
   m_comboBlocks.Detach();
}

LRESULT GameStringsView::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   DoDataExchange(DDX_LOAD);
   DlgResize_Init(false, false);

   m_listStrings.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT);

   CRect rect;
   m_listStrings.GetClientRect(&rect);

   int nWidth = 60;
   int nWidth2 = rect.Width()-nWidth-2-::GetSystemMetrics(SM_CXVSCROLL);

   m_listStrings.InsertColumn(0, _T("Nr."), LVCFMT_LEFT, nWidth, 0);
   m_listStrings.InsertColumn(1, _T("Text"), LVCFMT_LEFT, nWidth2, 0);

   return 0;
}

void GameStringsView::InitCombobox()
{
}

LRESULT GameStringsView::OnComboSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   int nItem = m_comboBlocks.GetCurSel();
   if (nItem == CB_ERR)
      return 0;

   Uint16 uiBlock = static_cast<Uint16>(m_comboBlocks.GetItemData(nItem) & 0xffff);

   RefreshStringList(uiBlock);

   return 0;
}

void GameStringsView::RefreshStringList(Uint16 /*uiBlock*/)
{
}
