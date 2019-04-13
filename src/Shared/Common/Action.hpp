//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Action.hpp Action
//
#pragma once

// includes
#include <vector>
#include "Object.hpp"
#include "ByteStream.hpp"

// forward references
class IModel;

/// action id
enum T_enActionId
{
   actionInvalid = 0x0000,

   actionSelectMobile = 0x0001,

   actionMobileActorEvaluate = 0x0002,

   // unit test actions
   actionDecreaseHealthPoints = 0x1000,
};

/// action class
class Action
{
public:
   /// ctor
   Action(unsigned short usActionId, ObjectId actorId, ObjectRef argumentRef)
      :m_usActionId(usActionId),
       m_actorId(actorId),
       m_argumentRef(argumentRef)
   {
   }

   /// dtor
   virtual ~Action() {}

   /// returns action id
   unsigned short ActionId() const { return m_usActionId; }

   /// returns unique id of object that is the actor of this action
   ObjectId& ActorId() { return m_actorId; }

   /// returns object ref of argument to this action
   ObjectRef& ArgumentRef() { return m_argumentRef; }


   /// carries out action with locked objects
   virtual void Do(IModel& model) = 0;

   // serialize

   /// serialize action by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const
   {
      stream.Write16(m_usActionId);

      stream.WriteUuid(m_actorId);
      stream.WriteUuid(m_argumentRef.m_id); // only serialize referenced id
   }

   /// deserialize action by reading bytes from stream
   virtual void Deserialize(ByteStream& stream)
   {
      m_usActionId = stream.Read16();

      m_actorId = stream.ReadUuid();
      m_argumentRef.m_id = stream.ReadUuid(); // only deserialize referenced id
      m_argumentRef.m_sp.reset();
   }

protected:
   /// action id; see T_enActionId
   unsigned short m_usActionId;

   /// actor object id
   ObjectId m_actorId;

   /// object ref of argument
   ObjectRef m_argumentRef;
};

/// action smart pointer
typedef std::shared_ptr<Action> ActionPtr;
