//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SafeFlag.hpp thread-safe flag
//
#pragma once

// includes
#include <atomic>

/// thread-safe set-only flag
class SafeFlag
{
public:
   /// ctor
   SafeFlag()
      :m_bFlag(false)
   {
   }

   /// returns if flag is already set
   bool IsSet() const
   {
      return m_bFlag;
   }

   /// sets flag
   void Set()
   {
      m_bFlag = true;
   }

   /// resets flag
   void Reset()
   {
      m_bFlag = false;
   }

private:
   /// flag
   std::atomic<bool> m_bFlag;
};
