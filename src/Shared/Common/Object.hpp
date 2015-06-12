//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Common/Object.hpp Object class
//
#pragma once

// includes
#include "Lockable.hpp"
#include "Common.hpp"
#include "Uuid.hpp"
#include "Vector3.hpp"
#include "MovementInfo.hpp"

// forward references
class ByteStream;

/// object id is implemented as unique id
typedef Uuid ObjectId;

/// \brief base class for all objects
class COMMON_DECLSPEC Object: public Lockable
{
public:
   /// ctor
   Object(const ObjectId& id)
      :m_id(id),
       m_pos(0.0, 0.0, 0.0)
   {
   }

   /// dtor
   virtual ~Object() throw() {}

   // getter

   /// unique object id
   const ObjectId& Id() const throw() { return m_id; }

   /// position
   const Vector3d& Pos() const throw() { return m_pos; }

   /// object name
   const CString& Name() const throw() { return m_cszName; }

   // setter

   /// position
   void Pos(const Vector3d& pos) throw() { m_pos = pos; }

   /// object name
   void Name(const CString& cszName) throw() { m_cszName = cszName; }

   // serialize

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream);

private:
   ObjectId m_id;       ///< object id
   Vector3d m_pos;      ///< position in world
   CString m_cszName;   ///< object name
};

/// object smart pointer
typedef std::shared_ptr<Object> ObjectPtr;


/// manages reference to an object by id, and may contain a shared ptr to the object itself
struct ObjectRef
{
   /// ctor; takes object id
   ObjectRef(ObjectId id)
      :m_id(id)
   {
   }

   /// ctor; takes object ptr (and sets id, too)
   ObjectRef(ObjectPtr sp)
      :m_id(sp->Id()),
       m_sp(sp)
   {
   }

   /// object id; is always set
   ObjectId m_id;

   /// object pointer; may be NULL
   ObjectPtr m_sp;
};
