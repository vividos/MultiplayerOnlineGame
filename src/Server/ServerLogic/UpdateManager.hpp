//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file UpdateManager.hpp Update manager
//
#pragma once

// includes
#include "ServerLogic.hpp"
#include "Object.hpp"
#include "ISessionManager.hpp"
#include "TimeBase.hpp"

// forward references
class MovementInfo;

/// \brief update manager for game state
/// \details This is probably the most important class in the server code; it
/// checks which connected players should get what updates. It has the
/// following features:
/// * Manages movement updates of players
/// * Manages action updates of players
/// * Manages visibility of players (who can see who)
/// * Manages rate of updates for all players
class UpdateManager
{
public:
   /// ctor
   UpdateManager(ISessionManager& sessionManager);
   /// dtor
   ~UpdateManager() {}

   /// shares movement update
   void ShareUpdateMovement(const ObjectId& objId, const MovementInfo& info);

   /// shares action
   void ShareAction(ActionPtr spAction);

   /// shares adding object
   void ShareAddObject(ObjectPtr spObj);

   /// shares removing object
   void ShareRemoveObject(const ObjectId& objId);

private:
   /// returns if two objects are in update distance
   bool InUpdateDistance(const ObjectId& objId1, ObjectPtr spObj2);

   /// returns if two objects are in update distance
   bool InUpdateDistance(const ObjectId& objId1, const ObjectId& objId2);

   /// type of last update
   enum T_enLastUpdateType
   {
      lastUpdateMovement,
      lastUpdateAction,
      lastUpdateAddRemoveObject,

      lastUpdateMax
   };

   /// check if update should be postponed (or dropped)
   bool CheckLastUpdate(const ObjectId& objId, T_enLastUpdateType lastAction);

private:
   /// session manager
   ISessionManager& m_sessionManager;

   /// player share info
   struct ShareInfo
   {
      /// ctor
      ShareInfo()
      {
         std::uninitialized_fill(std::begin(m_aLastUpdated), std::end(m_aLastUpdated), TimeIndex(0.0));
      }

      /// rough position of object
      Vector3d m_vPos;

      /// array with last updated time indices
      std::array<TimeIndex, lastUpdateMax> m_aLastUpdated;
   };

   /// share infos of all objects
   std::map<ObjectId, ShareInfo> m_mapShareInfo;
};
