//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ObjectMap.hpp Object map
//
#pragma once

// includes
#include <map>
#include <ulib/Exception.hpp>
#include "Object.hpp"

/// \brief object map
/// \details contains Object class instances
class ObjectMap
{
public:
   /// internal map type
   typedef std::map<ObjectId, ObjectPtr> T_mapObjects;

   /// internal map iterator type
   typedef T_mapObjects::iterator iterator;

   /// internal map const iterator type
   typedef T_mapObjects::const_iterator const_iterator;

   /// adds new object; throws when object is already in map
   void AddObject(ObjectPtr ptr)
   {
      T_mapObjects::const_iterator iter = m_mapObjects.find(ptr->Id());
      if (iter != m_mapObjects.end())
         throw Exception(_T("object already in map"), __FILE__, __LINE__);

      m_mapObjects.insert(std::make_pair(ptr->Id(), ptr));
   }

   /// checks if an object with given id is in map
   bool IsInMap(ObjectId id) const throw()
   {
      T_mapObjects::const_iterator iter = m_mapObjects.find(id);
      return iter != m_mapObjects.end();
   }

   /// finds object with given id in map; throws when object isn't in map
   ObjectRef FindObject(ObjectId id) const
   {
      T_mapObjects::const_iterator iter = m_mapObjects.find(id);
      if (iter == m_mapObjects.end())
         throw Exception(_T("couldn't locate object with id=") + id.ToString(), __FILE__, __LINE__);

      ObjectRef ref(iter->second->Id());
      ref.m_sp = iter->second;
      return ref;
   }

   /// removes object with given id from map; throws when object isn't in map
   void RemoveObject(ObjectId id)
   {
      T_mapObjects::iterator iter = m_mapObjects.find(id);
      if (iter == m_mapObjects.end())
         throw Exception(_T("couldn't remove object with id=") + id.ToString(), __FILE__, __LINE__);

      m_mapObjects.erase(iter);
   }

   /// returns internal map reference
   T_mapObjects& GetMap() throw() { return m_mapObjects; }

   /// returns internal map reference; const version
   const T_mapObjects& GetMap() const throw() { return m_mapObjects; }

private:
   /// object map
   T_mapObjects m_mapObjects;
};
