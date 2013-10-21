//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file AddRemoveObjectMessage.hpp Add/remove object message
//
#pragma once

// includes
#include "Network.hpp"
#include "Object.hpp"
#include "Message.hpp"
#include <vector>

/// message to tell client about added or removed objects
class NETWORK_DECLSPEC AddRemoveObjectMessage : public Message
{
public:
   /// ctor; use for receiving message
   AddRemoveObjectMessage()
      :Message(msgAddRemoveObject)
   {
   }

   /// ctor; use for sending message
   AddRemoveObjectMessage(const std::vector<ObjectPtr>& vecObjectsToAdd,
         const std::vector<ObjectId>& vecObjectsToRemove)
      :Message(msgAddRemoveObject),
       m_vecObjectsToAdd(vecObjectsToAdd.begin(), vecObjectsToAdd.end()),
       m_vecObjectsToRemove(vecObjectsToRemove.begin(), vecObjectsToRemove.end())
   {
   }

   /// dtor
   virtual ~AddRemoveObjectMessage() throw() {}

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override;

   // get methods

   /// returns list of objects to add
   const std::vector<ObjectPtr>& GetObjectsToAdd() const throw() { return m_vecObjectsToAdd; }

   /// returns list of objects to remove
   const std::vector<ObjectId>& GetObjectsToRemove() const throw() { return m_vecObjectsToRemove; }

private:
   /// list of objects to add
   std::vector<ObjectPtr> m_vecObjectsToAdd;

   /// list of objects to remove
   std::vector<ObjectId> m_vecObjectsToRemove;
};
