//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file SafeFlag.hpp thread-safe flag
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
   bool IsSet() const throw()
   {
      return m_bFlag;
   }

   /// sets flag
   void Set() throw()
   {
      m_bFlag = true;
   }

   /// resets flag
   void Reset() throw()
   {
      m_bFlag = false;
   }

private:
   /// flag
   std::atomic<bool> m_bFlag;
};
