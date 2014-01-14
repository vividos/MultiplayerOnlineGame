//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file MobileActor.cpp Mobile actor
//

// includes
#include "StdAfx.h"
#include "MobileActor.hpp"
#include "ThreatList.hpp"

MobileActor::MobileActor(const ObjectId& id) throw()
:Mobile(id),
 m_spThreatList(new ThreatList)
{
}

void MobileActor::Think(IModel& /*model*/, IActionQueue& /*actionQueue*/)
{
}
