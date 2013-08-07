//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file CrashReporter.hpp Application crash reporting
//
#pragma once

// includes
#include "Common.hpp"

/// crash reporter helper class
class COMMON_DECLSPEC CrashReporter
{
public:
   /// sets up crash reporting
   static void Init(const CString& cszBasePath);
};
