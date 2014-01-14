//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Command.hpp Command class
//
#pragma once

// includes
#include "Object.hpp"
#include "ByteStream.hpp"

/// command id
enum T_enCommandId
{
   cmdNull = 0x0000,    ///< no operation command
   cmdSelectObject,     ///< select object
   cmdHitMobile,        ///< hits mobile with main weapon
   cmdCastSpell,        ///< casts spell on object
};

/// player command
class Command
{
public:
   /// ctor
   Command(unsigned short id, ObjectId actorId, ObjectId argumentId)
      :m_id(id),
       m_actorId(actorId),
       m_argumentId(argumentId)
   {
   }

   // get methods

   /// returns command id; see T_enCommandId
   unsigned short Id() const throw() { return m_id; }

   /// returns command actor id
   ObjectId ActorId() const throw() { return m_actorId; }

   /// returns id of argument the command is acting on
   ObjectId ArgumentId() const throw() { return m_argumentId; }

   // set methods

   /// sets actor id
   void ActorId(ObjectId actorId){ m_actorId = actorId; }

   // serializing

   /// serialize message by putting bytes to stream
   void Serialize(ByteStream& stream) const
   {
      ATLASSERT(m_id < std::numeric_limits<unsigned short>::max());
      stream.Write16(static_cast<unsigned short>(m_id));
      // stream.WriteUuid(m_actorId); // omit writing actor id; can be set by receiver
      stream.WriteUuid(m_argumentId);
   }

   /// deserialize message by reading bytes from stream
   void Deserialize(ByteStream& stream)
   {
      m_id = stream.Read16();
      m_actorId = ObjectId::Null();
      m_argumentId = stream.ReadUuid();
   }

private:
   /// command id; see T_enCommandId
   unsigned short m_id;

   /// command actor id
   ObjectId m_actorId;

   /// id of argument the command is acting on
   ObjectId m_argumentId;
};
