//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Debugger/MainFrame.cpp Main application frame
//

// includes

// includes
#include "stdafx.h"
#include "MainFrame.hpp"
#include "GameStringsView.hpp"

LRESULT MainFrame::OnViewHotspotList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   bool bVisible = ShowHideDockingWindow(m_hotspotListWindow);
   UISetCheck(ID_VIEW_HOTSPOT, bVisible);
   return 0;
}

LRESULT MainFrame::OnViewGameStrings(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   GameStringsViewChildFrame* pChild = new GameStringsViewChildFrame(m_client);
   pChild->CreateEx(m_hWndClient);

   MDIMaximize(pChild->m_hWnd);

   return 0;
}
