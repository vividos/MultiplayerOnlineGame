//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file ThreatList.hpp Threat list
//
#pragma once

// includes
#include "World.hpp"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include "Object.hpp"

/// list of mobiles that caused threat
class WORLD_DECLSPEC ThreatList
{
public:
   /// ctor
   ThreatList() throw()
   {
   }

   /// outcome for theat list action
   enum T_enThreatListOutcome
   {
      threatListAdded,        ///< entry was newly added
      threatListRemoved,      ///< entry was removed
      threatListOrderChanged, ///< order was changed
      threatListNothing,      ///< nothing was changed
   };

   // get methods

   /// returns if threat list is empty
   bool IsEmpty() const throw();

   /// checks if an object is in the list
   bool IsInList(ObjectId id) const throw();

   // action methods

   /// adds theat to a new or existing entry
   T_enThreatListOutcome Add(ObjectId id, signed int uiThreat);

   /// removes specific entry from the list
   void Remove(ObjectId id);

   /// removes all entries from the list
   void RemoveAll();

   /// returns top entry of threat list, or ObjectId::Null()
   ObjectId Top();

private:
   /// threat list entry
   struct Entry
   {
      /// ctor
      Entry(ObjectId id, unsigned int uiThreat = 0)
         :m_id(id),
          m_uiThreat(uiThreat)
      {
      }

      /// mobile's id
      ObjectId m_id;

      /// threat
      unsigned int m_uiThreat;

      /// less operator to order by threat
      bool operator<(const Entry& rhs) const throw()
      {
         // we want bigger threat entries first in ordering
         return m_uiThreat >= rhs.m_uiThreat;
      }
   };

   /// threat list map
   typedef boost::multi_index_container<Entry,
      boost::multi_index::indexed_by<
         // ordered by id
         boost::multi_index::ordered_unique<
            boost::multi_index::member<Entry, ObjectId, &Entry::m_id> >,
         // ordered by operator< or threat
         boost::multi_index::ordered_non_unique<
            boost::multi_index::identity<Entry> >
      >
   > T_MapThreatList;

   /// threat list
   T_MapThreatList m_mapThreatList;

private:
   /// calculate new threat value
   unsigned int CalcNewThreat(const Entry& entry, signed int siThreat);

   // determine outcome of reordering when re-inserting entry
   T_enThreatListOutcome DetermineReorderOutcome(T_MapThreatList::iterator iter,
      signed int siThreat, unsigned int uiNewThreat);
};
