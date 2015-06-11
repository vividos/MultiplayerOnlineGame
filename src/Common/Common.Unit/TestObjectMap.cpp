//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file TestObjectMap.cpp Unit tests for ObjectMap class
//

// includes
#include "stdafx.h"
#include "ObjectMap.hpp"
#include <vector>
#include "Vector3.hpp"

namespace UnitTest
{
   // algorithms on ObjectMap

   /// finds all objects in map, filtering by predicate
   template <typename Predicate>
   void FindAllObjects(const ObjectMap& objectMap,
      std::vector<ObjectPtr>& vecObjects,
      Predicate pred) throw()
   {
      const ObjectMap::T_mapObjects& objMap = objectMap.GetMap();
      ObjectMap::T_mapObjects::const_iterator iter = objMap.begin(), stop = objMap.end();

      for (; iter != stop; ++iter)
      {
         if (!pred(iter->second))
            continue;

         vecObjects.push_back(iter->second);
      }
   }

   /// finds all objects in map, without object with given id
   void FindAllObjectsExcept(const ObjectMap& objectMap, const ObjectId& idExcept,
      std::vector<ObjectPtr>& vecObjects) throw()
   {
      const ObjectMap::T_mapObjects& objMap = objectMap.GetMap();
      ObjectMap::T_mapObjects::const_iterator iter = objMap.begin(), stop = objMap.end();

      for (; iter != stop; ++iter)
      {
         if (iter->first == idExcept)
            continue;

         vecObjects.push_back(iter->second);
      }
   }

   double Distance(const Vector3d& v1, const Vector3d& v2)
   {
      return (v1 - v2).Length();
   }

   /// finds all objects in map within given range
   void FindAllObjectsInRange(const ObjectMap& objectMap, const ObjectId& idExcept,
      double dRange, Vector3d vecPos,
      std::vector<ObjectPtr>& vecObjects)
   {
      const ObjectMap::T_mapObjects& objMap = objectMap.GetMap();
      ObjectMap::T_mapObjects::const_iterator iter = objMap.begin(), stop = objMap.end();

      for (; iter != stop; ++iter)
      {
         if (iter->first == idExcept)
            continue;

         if (Distance(vecPos, iter->second->Pos()) < dRange)
            vecObjects.push_back(iter->second);
      }
   }

   /// finds all objects added and removed from two given map states
   void FindAddedRemovedObjects(const ObjectMap& /*objectMapBefore*/, const ObjectMap& /*objectMapAfter*/,
      std::vector<ObjectPtr>& /*vecAddedObjects*/,
      std::vector<ObjectId>& /*vecRemovedObjectIds*/)
   {
      // TODO implement
   }


   /// tests class ObjectMap
   TEST_CLASS(TestObjectMap)
   {
      /// tests default ctor
      TEST_METHOD(TestDefaultCtor)
      {
         ObjectMap objMap1, objMap2;
      }
   };

} // namespace UnitTest
