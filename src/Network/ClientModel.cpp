//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ClientModel.cpp Client model
//

// includes
#include "stdafx.h"
#include "ClientModel.hpp"

void ClientModel::Tick(const TimeIndex&)
{
   // no tick processing; the server does this for us
}

bool ClientModel::OnReceiveMessage(RawMessage& /*msg*/)
{
   // TODO
   return false;
}
