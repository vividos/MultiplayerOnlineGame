//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file UpdateObjectMovementMessage.hpp Message to update object movement
//
#pragma once

// includes
#include "Network.hpp"
#include "Message.hpp"
#include "Object.hpp"
#include "MovementInfo.hpp"

/// message for updating object movement info; sent to the client
class NETWORK_DECLSPEC UpdateObjectMovementMessage : public Message
{
public:
   /// ctor; use for sending
   UpdateObjectMovementMessage(const ObjectId& objId, const MovementInfo& movementInfo)
      :Message(msgUpdateObjectMovement),
       m_objId(objId),
       m_movementInfo(movementInfo)
   {
   }

   /// ctor; use for receiving
   UpdateObjectMovementMessage()
      :Message(msgUpdateObjectMovement),
       m_objId(ObjectId::Null())
   {
   }

   /// dtor
   virtual ~UpdateObjectMovementMessage() {}

   // get methods

   /// returns object id
   const ObjectId& Id() const { return m_objId; }

   /// returns movement info
   const MovementInfo& Info() const { return m_movementInfo; }


   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override;

private:
   /// object id
   ObjectId m_objId;

   /// movement info
   MovementInfo m_movementInfo;
};
