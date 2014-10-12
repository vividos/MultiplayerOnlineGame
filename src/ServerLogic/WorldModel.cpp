//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file WorldModel.cpp World model
//

// includes
#include "StdAfx.h"
#include "WorldModel.hpp"
#include "IActionQueue.hpp"

void WorldModel::InitialUpdate(MobilePtr spPlayer)
{
   // doesn't occur
   ATLASSERT(false);
}

void WorldModel::Tick(const TimeIndex& /*timeIndex*/)
{
   // TODO call storyboard
}

void WorldModel::ReceiveAction(ActionPtr spAction)
{
   m_actionQueue.Post(spAction);

   // determine who should get update message
   m_updateManager.ShareAction(spAction);
}

/// \details receives command from every server side model
void WorldModel::ReceiveCommand(Command& c)
{
   m_commandTranslator.ReceiveCommand(c);
}

/// \details used by session manager to add and remove players
void WorldModel::AddRemoveObject(const std::vector<ObjectPtr>& vecObjectsToAdd,
   const std::vector<ObjectId>& vecObjectsToRemove)
{
   for (size_t i=0,iMax=vecObjectsToRemove.size(); i<iMax; i++)
   {
      const ObjectId& objId = vecObjectsToRemove[i];

      {
         RecursiveMutex::LockType lock(m_mtxObjectMap);
         m_objectMap.RemoveObject(objId);
      }

      m_updateManager.ShareRemoveObject(objId);
   }

   for (size_t i=0,iMax=vecObjectsToAdd.size(); i<iMax; i++)
   {
      const ObjectPtr& spObj = vecObjectsToAdd[i];

      {
         RecursiveMutex::LockType lock(m_mtxObjectMap);
         m_objectMap.AddObject(spObj);
      }

      m_updateManager.ShareAddObject(spObj);
   }
}

/// \details called when an object updated its position or movement info
void WorldModel::UpdateObjectMovement(const ObjectId& id, const MovementInfo& info)
{
   CString cszText;
   cszText.Format(_T("got update message from: %s"), id.ToString());
   LOG_INFO(cszText, Log::Server::World::Player);

   // determine who should get update message
   m_updateManager.ShareUpdateMovement(id, info);
}

void WorldModel::QueueAction(ActionPtr spAction)
{
   m_actionQueue.Post(spAction);
}
