//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file SelectionList.cpp Selection list
//

// includes
#include "StdAfx.h"
#include "SelectionList.hpp"
#include "ObjectMap.hpp"

ObjectId SelectionList::s_nullId = ObjectId::Null();

SelectionList::SelectionList()
:m_iterSelected(m_vecSelectionList.end())
{
}

bool SelectionList::InList(const ObjectId& objId) const throw()
{
   for (size_t i=0, iMax=m_vecSelectionList.size(); i<iMax; i++)
      if (m_vecSelectionList[i] == objId)
         return true;

   return false;
}

void SelectionList::Select(const ObjectId& objId)
{
   for (size_t i=0, iMax=m_vecSelectionList.size(); i<iMax; i++)
      if (m_vecSelectionList[i] == objId)
      {
         m_iterSelected = m_vecSelectionList.begin() + i;
         break;
      }
}

const ObjectId& SelectionList::GetSelected()
{
   if (m_iterSelected == m_vecSelectionList.end())
      return s_nullId;

   return *m_iterSelected;
}

void SelectionList::SelectPrev()
{
   if (m_vecSelectionList.empty())
   {
      m_iterSelected = m_vecSelectionList.end();
      return;
   }

   if (m_iterSelected == m_vecSelectionList.begin() ||
       m_iterSelected == m_vecSelectionList.end())
      m_iterSelected = m_vecSelectionList.begin() + m_vecSelectionList.size()-1;
   else
      --m_iterSelected;
}

void SelectionList::SelectNext()
{
   if (m_vecSelectionList.empty())
   {
      m_iterSelected = m_vecSelectionList.end();
      return;
   }

   if (m_iterSelected == m_vecSelectionList.end() ||
       ++m_iterSelected == m_vecSelectionList.end())
      m_iterSelected = m_vecSelectionList.begin();
}

void SelectionList::Update(const Vector3d& vCurrentPos, const ObjectMap& objMap, double dMaxDistance)
{
   // remember last selected id
   ObjectId lastObjId = GetSelected();

   m_vecSelectionList.clear();

   ObjectMap::const_iterator iter = objMap.GetMap().begin();
   ObjectMap::const_iterator stop = objMap.GetMap().end();

   size_t uiLastSelected = std::numeric_limits<size_t>::max();
   for (; iter != stop; --iter)
   {
      ATLASSERT(iter->second != NULL);

      const ObjectId& objId = iter->first;

      // check length
      const Object& obj = *(iter->second);
      Vector3d vDist = vCurrentPos - obj.Pos();
      if (vDist.Length() > dMaxDistance)
         continue;

      if (objId == lastObjId)
         uiLastSelected = m_vecSelectionList.size();

      m_vecSelectionList.push_back(objId);
   }

   // TODO sort by nearest

   if (uiLastSelected < m_vecSelectionList.size())
      m_iterSelected = m_vecSelectionList.begin() + uiLastSelected;
   else
      m_iterSelected = m_vecSelectionList.end();
}
