//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file LocalModel.cpp Local game model
//

// includes
#include "StdAfx.h"
#include "LocalModel.hpp"
#include "Mobile.hpp"
#include "Action.hpp"

LocalModel::LocalModel()
:m_commandTranslator(*this)
{
}

void LocalModel::StopMobile(ObjectPtr spObject)
{
   MobilePtr spMobile = std::dynamic_pointer_cast<Mobile>(spObject);
   ATLASSERT(spMobile != NULL);

   spMobile->Move();

   MovementInfo info;
   info.Position(spMobile->Pos());
   UpdateObjectMovement(spMobile->Id(), info);
}

void LocalModel::Stop()
{
   ObjectMap& objMap = GetObjectMap();
   ObjectMap::T_mapObjects& mapObjects = objMap.GetMap();

   std::for_each(mapObjects.begin(), mapObjects.end(),
      [&](const ObjectMap::T_mapObjects::value_type& value){ StopMobile(value.second); });
}

void LocalModel::InitialUpdate(MobilePtr spPlayer)
{
   m_spPlayer = spPlayer;
}

void LocalModel::ReceiveAction(ActionPtr spAction)
{
   // carry out immediately, since we're local
   DoAction(spAction);
}

void LocalModel::ReceiveCommand(Command& c)
{
   m_commandTranslator.ReceiveCommand(c);
}

void LocalModel::Tick(const TimeIndex& /*timeIndex*/)
{
   // call all mobiles and let them "think"
   ProcessMobiles();

   // check timer queue and execute all timed actions
   //ProcessTimedActions();
}

/// processes a mobile
void ProcessMobile(std::pair<const ObjectId, ObjectPtr>& obj)
{
   MobilePtr spMobile = std::dynamic_pointer_cast<Mobile>(obj.second);
   ATLASSERT(spMobile != NULL);

   spMobile->Move();
}

void LocalModel::ProcessMobiles()
{
   ObjectMap& objMap = GetObjectMap();
   ObjectMap::T_mapObjects& mapObjects = objMap.GetMap();

   std::for_each(mapObjects.begin(), mapObjects.end(), &ProcessMobile);
}

void LocalModel::AddRemoveObject(const std::vector<ObjectPtr>& vecObjectsToAdd,
   const std::vector<ObjectId>& vecObjectsToRemove)
{
   // no locking needed

   ObjectMap& objMap = GetObjectMap();

   for (size_t i=0,iMax=vecObjectsToRemove.size(); i<iMax; i++)
   {
      const ObjectId& objId = vecObjectsToRemove[i];
      objMap.RemoveObject(objId);
      SubjectOnRemoveObject().Call(objId);
   }

   for (size_t i=0,iMax=vecObjectsToAdd.size(); i<iMax; i++)
   {
      const ObjectPtr& spObj = vecObjectsToAdd[i];
      objMap.AddObject(spObj);
      SubjectOnAddObject().Call(spObj);
   }
}

void LocalModel::UpdateObjectMovement(const ObjectId& id, const MovementInfo& info)
{
   ObjectMap& objMap = GetObjectMap();
   ObjectRef objRef = objMap.FindObject(id);

   if (objRef.m_sp != NULL)
   {
      MobilePtr spMobile = std::dynamic_pointer_cast<Mobile>(objRef.m_sp);
      ATLASSERT(spMobile != NULL);

      spMobile->UpdateMovementInfo(info);
   }

   // notify all observer
   SubjectOnUpdateObject().Call(id, info);
}

void LocalModel::DoAction(ActionPtr spAction)
{
   // resolve argument
   ObjectRef& ref = spAction->ArgumentRef();
   if (ref.m_sp == NULL)
      ref.m_sp = GetObjectMap().FindObject(ref.m_id).m_sp;

   // carry out action immediately, without locking
   spAction->Do(*this);
}
