//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Debugger/MainFrame.hpp Main application frame
//
#pragma once

// includes
#include "MainFrameBase.hpp"
#include "HotspotListWindow.hpp"

/// debugger main frame
class MainFrame : public MainFrameBase
{
   typedef MainFrameBase baseClass;

public:
   /// ctor
   MainFrame(DebugClient& client)
      :m_client(client),
      m_hotspotListWindow(client)
   {
   }

private:
   BEGIN_UPDATE_UI_MAP(MainFrame)
      UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
      UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
      UPDATE_ELEMENT(ID_VIEW_HOTSPOT, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
      UPDATE_ELEMENT(ID_VIEW_GAMESTRINGS, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
   END_UPDATE_UI_MAP()

   BEGIN_MSG_MAP(MainFrameBase)
      COMMAND_ID_HANDLER(ID_VIEW_HOTSPOT, OnViewHotspotList)
      COMMAND_ID_HANDLER(ID_VIEW_GAMESTRINGS, OnViewGameStrings)
      CHAIN_MDI_CHILD_COMMANDS()
      CHAIN_MSG_MAP(baseClass)
      REFLECT_NOTIFICATIONS()
   END_MSG_MAP()

   LRESULT OnViewHotspotList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
   LRESULT OnViewGameStrings(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
   /// debug client
   DebugClient& m_client;

   // docking windows

   /// hotspot list
   HotspotListWindow m_hotspotListWindow;
};
