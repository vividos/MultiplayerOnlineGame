//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file TimeBase.cpp Game time base
//

// includes
#include "StdAfx.h"
#include "TimeBase.hpp"

TimeBase::TimeBase() throw()
:m_dEpochOffset(0.0)
{
   m_timer.Start();
}

TimeIndex TimeBase::Now() const throw()
{
   return TimeIndex(m_dEpochOffset + m_timer.Elapsed());
}
