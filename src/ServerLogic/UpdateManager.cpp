//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file UpdateManager.cpp Update manager
//

// includes
#include "StdAfx.h"
#include "UpdateManager.hpp"
#include "Action.hpp"
#include "Session.hpp"
#include "MovePlayerMessage.hpp"
#include "ActionMessage.hpp"
#include "AddRemoveObjectMessage.hpp"

UpdateManager::UpdateManager(ISessionManager& sessionManager)
:m_sessionManager(sessionManager)
{
}

void UpdateManager::ShareUpdateMovement(const ObjectId& objId, const MovementInfo& info)
{
   // go through all objects and check if they should be updated
   std::map<ObjectId, ShareInfo>::iterator iter = m_mapShareInfo.begin(), stop = m_mapShareInfo.end();
   for (; iter != stop; ++iter)
   {
      if (iter->first == objId)
         continue; // no need to update self

      if (!InUpdateDistance(iter->first, objId))
         continue;

      if (CheckLastUpdate(iter->first, lastUpdateMovement))
      {
         // ok, send action
         std::weak_ptr<Session> wpSession =
            m_sessionManager.FindSession(iter->first);

         std::shared_ptr<Session> spSession = wpSession.lock();
         if (spSession == NULL)
            continue;

         MovePlayerMessage actionMessage(info);
         spSession->SendMessage(actionMessage);
      }
      else
      {
         // TODO: postpone update
         ATLASSERT(false);
      }
   }
}

void UpdateManager::ShareAction(ActionPtr spAction)
{
   const ObjectId& objId = spAction->ActorId();

   // go through all objects and check if they should be updated
   std::map<ObjectId, ShareInfo>::iterator iter = m_mapShareInfo.begin(), stop = m_mapShareInfo.end();
   for (; iter != stop; ++iter)
   {
      if (iter->first == objId)
         continue; // no need to update self

      if (!InUpdateDistance(iter->first, objId))
         continue;

      if (CheckLastUpdate(iter->first, lastUpdateAction))
      {
         // ok, send action
         std::weak_ptr<Session> wpSession =
            m_sessionManager.FindSession(iter->first);

         std::shared_ptr<Session> spSession = wpSession.lock();
         if (spSession == NULL)
            continue;

         ActionMessage actionMessage(spAction);
         spSession->SendMessage(actionMessage);
      }
      else
      {
         // TODO: postpone update
         ATLASSERT(false);
      }
   }
}

void UpdateManager::ShareAddObject(ObjectPtr spObj)
{
   // go through all objects and check if they should be updated
   std::map<ObjectId, ShareInfo>::iterator iter = m_mapShareInfo.begin(), stop = m_mapShareInfo.end();
   for (; iter != stop; ++iter)
   {
      ATLASSERT(iter->first != spObj->Id()); // must not be in map

      if (!InUpdateDistance(iter->first, spObj))
         continue;

      if (CheckLastUpdate(iter->first, lastUpdateAddRemoveObject))
      {
         // update object
         std::weak_ptr<Session> wpSession =
            m_sessionManager.FindSession(iter->first);

         std::shared_ptr<Session> spSession = wpSession.lock();
         if (spSession == NULL)
            continue;

         std::vector<ObjectPtr> vecObjectsToAdd;
         vecObjectsToAdd.push_back(spObj);

         std::vector<ObjectId> vecObjectsToRemove;

         AddRemoveObjectMessage msg(vecObjectsToAdd, vecObjectsToRemove);
         spSession->SendMessage(msg);
      }
      else
      {
         // TODO: postpone update
         ATLASSERT(false);
      }
   }

   // add to map
   ShareInfo info;
   info.m_vPos = spObj->Pos();

   ATLASSERT(m_mapShareInfo.find(spObj->Id()) == m_mapShareInfo.end()); // must not be in map

   m_mapShareInfo.insert(std::make_pair(spObj->Id(), info));
}

void UpdateManager::ShareRemoveObject(const ObjectId& objId)
{
   ATLASSERT(m_mapShareInfo.find(objId) != m_mapShareInfo.end()); // must be in map

   // remove from map
   m_mapShareInfo.erase(objId);

   // go through all objects and check if they should be updated
   std::map<ObjectId, ShareInfo>::iterator iter = m_mapShareInfo.begin(), stop = m_mapShareInfo.end();
   for (; iter != stop; ++iter)
   {
      if (!InUpdateDistance(iter->first, objId))
         continue;

      if (CheckLastUpdate(iter->first, lastUpdateAddRemoveObject))
      {
         // update object
         std::weak_ptr<Session> wpSession =
            m_sessionManager.FindSession(iter->first);

         std::shared_ptr<Session> spSession = wpSession.lock();
         if (spSession == NULL)
            continue;

         std::vector<ObjectPtr> vecObjectsToAdd;
         std::vector<ObjectId> vecObjectsToRemove;
         vecObjectsToRemove.push_back(objId);

         AddRemoveObjectMessage msg(vecObjectsToAdd, vecObjectsToRemove);
         spSession->SendMessage(msg);
      }
      else
      {
         // TODO: postpone update
         ATLASSERT(false);
      }
   }
}

bool UpdateManager::InUpdateDistance(const ObjectId& objId1, ObjectPtr spObj2)
{
   ATLASSERT(m_mapShareInfo.find(objId1) != m_mapShareInfo.end());

   const ShareInfo& info1 = m_mapShareInfo[objId1];

   return (info1.m_vPos - spObj2->Pos()).Length() < c_dMaxVisibleDistance;
}

bool UpdateManager::InUpdateDistance(const ObjectId& objId1, const ObjectId& objId2)
{
   ATLASSERT(m_mapShareInfo.find(objId1) != m_mapShareInfo.end());
   ATLASSERT(m_mapShareInfo.find(objId2) != m_mapShareInfo.end());

   const ShareInfo& info1 = m_mapShareInfo[objId1];
   const ShareInfo& info2 = m_mapShareInfo[objId2];

   return (info1.m_vPos - info2.m_vPos).Length() < c_dMaxVisibleDistance;
}

bool UpdateManager::CheckLastUpdate(const ObjectId& /*objId*/, T_enLastUpdateType /*enLastAction*/)
{
   return true;
}
