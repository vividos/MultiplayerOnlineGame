//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file HotspotListWindow.hpp Hotspot list
//
#pragma once

// includes
#include "DockingWindowBase.hpp"

// forward references
class DebugClient;

/// hotspot list docking window
class HotspotListWindow : public DockingWindowBase
{
   typedef HotspotListWindow thisClass;
   typedef DockingWindowBase baseClass;
public:
   /// ctor
   HotspotListWindow(DebugClient& client)
      :m_client(client)
   {
   }
   /// dtor
   virtual ~HotspotListWindow() throw() {}

   DECLARE_DOCKING_WINDOW(_T("Hotspot List"), CSize(250,100)/*docked*/, CSize(450,300)/*floating*/, dockwins::CDockingSide::sLeft)

   DECLARE_WND_CLASS(_T("HotspotListWindow"))

   // message map
   BEGIN_MSG_MAP(thisClass)
      ATLASSERT_ADDED_REFLECT_NOTIFICATIONS()
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
      NOTIFY_CODE_HANDLER(NM_DBLCLK, OnDblClick);
      CHAIN_MSG_MAP(baseClass)
   END_MSG_MAP()

protected:
   // message handler
   LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
   LRESULT OnDblClick(WPARAM /*wParam*/, NMHDR* pNMHDR, BOOL& /*bHandled*/);

protected:
   /// debug client
   DebugClient& m_client;

   /// list with all hotspots
   CListViewCtrl m_listCtrl;
};
