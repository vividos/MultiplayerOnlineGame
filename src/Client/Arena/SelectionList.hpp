//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SelectionList.hpp Selection list
//
#pragma once

// includes
#include "ClientLogic.hpp"
#include "Object.hpp"
#include "Vector3.hpp"

// forward referces
class ObjectMap;

/// list of selectable mobiles in range
class SelectionList
{
public:
   /// ctor
   SelectionList();

   /// returns if selection is valid (or null)
   bool IsSelectionValid() const;

   /// returns if object is in selection list
   bool InList(const ObjectId& objId) const;

   /// selects object
   void Select(const ObjectId& objId);

   /// returns currently selected id
   const ObjectId& GetSelected();

   /// selects previous object in list
   void SelectPrev();

   /// selects next object in list
   void SelectNext();

   /// updates list, keeping selected object, if still in range
   void Update(const Vector3d& vCurrentPos, const ObjectMap& objMap, double dMaxDistance);

private:
   /// selection list
   std::vector<ObjectId> m_vecSelectionList;

   /// iterator to selection list of selected object
   std::vector<ObjectId>::const_iterator m_iterSelected;

   /// null id
   static ObjectId s_nullId;
};
