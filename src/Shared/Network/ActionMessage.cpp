//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ActionMessage.cpp Action message
//

// includes
#include "StdAfx.h"
#include "ActionMessage.hpp"
#include "Action.hpp"
#include "ByteStream.hpp"
#include "ActionRegistry.hpp"

void ActionMessage::Serialize(ByteStream& stream) const
{
   ATLASSERT(m_spAction != NULL); // action must have been passed in ctor

   stream.Write16(m_spAction->ActionId());

   m_spAction->Serialize(stream);
}

void ActionMessage::Deserialize(ByteStream& stream)
{
   ATLASSERT(m_spAction == NULL); // action passed in ctor must be empty

   // read action id
   unsigned short usActionId = stream.Read16();

   // construct message by action registry
   m_spAction = ActionRegistry::CreateById(usActionId);
   m_spAction->Deserialize(stream);
}
