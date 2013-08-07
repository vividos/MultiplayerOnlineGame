//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file AddRemoveObjectMessage.cpp Add/remove object message
//

// includes
#include "StdAfx.h"
#include "AddRemoveObjectMessage.hpp"
#include "ByteStream.hpp"
#include "Mobile.hpp"

void AddRemoveObjectMessage::Serialize(ByteStream& stream) const
{
   // list of objects to add
   stream.Write32(m_vecObjectsToAdd.size());
   for (size_t i=0, iMax=m_vecObjectsToAdd.size(); i<iMax; i++)
   {
      ATLASSERT(typeid(Mobile) == typeid(*m_vecObjectsToAdd[i]));
      m_vecObjectsToAdd[i]->Serialize(stream);
   }

   // list of objects to remove
   stream.Write32(m_vecObjectsToRemove.size());

   for (size_t i=0, iMax=m_vecObjectsToRemove.size(); i<iMax; i++)
      stream.WriteUuid(m_vecObjectsToRemove[i]);
}

void AddRemoveObjectMessage::Deserialize(ByteStream& stream)
{
   // list of objects to add
   size_t iMax = stream.Read32();
   m_vecObjectsToAdd.reserve(iMax);
   for (size_t i=0; i<iMax; i++)
   {
      MobilePtr spMobile(new Mobile(Uuid::Null()));
      spMobile->Deserialize(stream);

      m_vecObjectsToAdd.push_back(spMobile);
   }

   iMax = stream.Read32();
   m_vecObjectsToRemove.reserve(iMax);
   for (size_t i=0; i<iMax; i++)
      m_vecObjectsToRemove.push_back(stream.ReadUuid());
}
