//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2014 Michael Fink
//
/// \file Underworld/Debugger/stdafx.h Precompiled header support
//
#pragma once

// includes
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#include <ulib/config/Wtl.hpp>

// runtime-check safe macros for COLORREF
#undef GetRValue
#undef GetGValue
#undef GetBValue
#define GetRValue(rgb)      ((BYTE)(rgb&0xFF))
#define GetGValue(rgb)      ((BYTE)((((WORD)(rgb&0xFFFF)) >> 8)&0xFF))
#define GetBValue(rgb)      ((BYTE)(((rgb)>>16)&0xFF))

#include "Base.hpp"
#include "UwBase.hpp"

// extra ATL includes
#include <atlcoll.h>

// extra WTL includes
#include <atlcrack.h>

// Tabbed Framework includes
#include <atlgdix.h>
#include <TabbedMDI.h>

// Docking Framework includes
#pragma warning(disable: 4100) // unreferenced formal parameter
#include <DockMisc.h>
#include <ExtDockingWindow.h>
#include <DockingFrame.h>
#include <DotNetTabCtrl.h>
#include <CustomTabCtrl.h>
#include <TabbedFrame.h>
#include <TabbedDockingWindow.h>
#include <DockingBox.h>
#include <TabDockingBox.h>
#include <VC7LikeCaption.h>
#pragma warning(default: 4100)

// Menu XP includes
#include "MenuXP.h"

/// check macro to check if the REFLECT_NOTIFICATIONS() macro was added to the parent class
#define ATLASSERT_ADDED_REFLECT_NOTIFICATIONS() CheckAddedReflectNotifications(hWnd, uMsg, wParam, lParam);

/// helper function to check if the REFLECT_NOTIFICATIONS() macro was added to the parent class
extern void CheckAddedReflectNotifications(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


/// macro to route individual command messages to member variables
#define COMMAND_ROUTE_TO_MEMBER(uCmd, m_Member) \
   if (uMsg == WM_COMMAND && LOWORD(wParam) == uCmd) \
      CHAIN_MSG_MAP_MEMBER(m_Member)
