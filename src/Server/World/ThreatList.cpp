//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ThreatList.cpp Threat list
//

// includes
#include "StdAfx.h"
#include "ThreatList.hpp"

bool ThreatList::IsEmpty() const
{
   return m_mapThreatList.empty();
}

bool ThreatList::IsInList(ObjectId id) const
{
   return m_mapThreatList.find(id) != m_mapThreatList.end();
}

ThreatList::T_enThreatListOutcome ThreatList::Add(ObjectId id, signed int siThreat)
{
   T_MapThreatList::iterator iter = m_mapThreatList.find(id);

   if (iter == m_mapThreatList.end())
   {
      if (siThreat < 0)
         return threatListNothing;

      // new entry
      m_mapThreatList.insert(Entry(id, unsigned(siThreat)));
      return threatListAdded;
   }

   const Entry& entry = *iter;

   // check if we can remove
   if (siThreat < 0 && entry.m_uiThreat < unsigned(-siThreat))
   {
      // theat would drop to or below 0
      m_mapThreatList.erase(id);
      return threatListRemoved;
   }

   // at this point, the threat cannot drop below zero

   unsigned int uiNewThreat = CalcNewThreat(entry, siThreat);

   // determine outcome
   ThreatList::T_enThreatListOutcome enOutcome =
      DetermineReorderOutcome(iter, siThreat, uiNewThreat);

   // re-insert entry in any case, even if order isn't changed (since
   // multi index container has non-mutable elements)
   {
      Entry newEntry(entry.m_id, uiNewThreat);

      m_mapThreatList.erase(id);

      m_mapThreatList.insert(Entry(id, unsigned(siThreat)));
   }

   return enOutcome;
}

void ThreatList::Remove(ObjectId id)
{
   ATLASSERT(IsInList(id) == true);

   m_mapThreatList.erase(id);
}

void ThreatList::RemoveAll()
{
   m_mapThreatList.clear();
}

ObjectId ThreatList::Top()
{
   if (m_mapThreatList.empty())
      return ObjectId::Null();

   return m_mapThreatList.get<1>().begin()->m_id;
}

unsigned int ThreatList::CalcNewThreat(const Entry& entry, signed int siThreat)
{
   unsigned int uiNewThreat = entry.m_uiThreat;
   if (siThreat > 0)
   {
      unsigned long long ullNewThreat = entry.m_uiThreat;
      ullNewThreat += unsigned(siThreat);
      if (ullNewThreat >= std::numeric_limits<unsigned int>::max())
         uiNewThreat = std::numeric_limits<unsigned int>::max();
      else
         uiNewThreat += unsigned(siThreat);
   }
   else
   if (siThreat < 0)
   {
      ATLASSERT(uiNewThreat >= unsigned(-siThreat));
      uiNewThreat -= unsigned(-siThreat);
   }

   return uiNewThreat;
}

ThreatList::T_enThreatListOutcome ThreatList::DetermineReorderOutcome(
   T_MapThreatList::iterator iter, signed int siThreat, unsigned int uiNewThreat)
{
   ThreatList::T_enThreatListOutcome enOutcome = threatListNothing;

   if (siThreat > 0)
   {
      T_MapThreatList::nth_index<1>::type::iterator prev =
         m_mapThreatList.project<1>(iter);

      // already at top of list?
      if (prev != m_mapThreatList.get<1>().begin())
      {
         // no, check previous
         prev--;

         // check threat values
         if (uiNewThreat > prev->m_uiThreat)
            enOutcome = threatListOrderChanged;
      }
   }
   else
   if (siThreat < 0)
   {
      T_MapThreatList::nth_index<1>::type::iterator next =
         m_mapThreatList.project<1>(iter);
      next++;

      // already at end of list?
      if (next != m_mapThreatList.get<1>().end())
      {
         // no, check threat values
         if (uiNewThreat < next->m_uiThreat)
            enOutcome = threatListOrderChanged;
      }
   }

   return enOutcome;
}
