//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file CrashReporter.hpp Application crash reporting
//
#pragma once

// includes
#include "Base.hpp"

/// crash reporter helper class
class BASE_DECLSPEC CrashReporter
{
public:
   /// sets up crash reporting
   static void Init(const CString& cszBasePath);
};
