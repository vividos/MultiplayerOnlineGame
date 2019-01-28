//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file WorldModel.hpp World model
//
#pragma once

// includes
#include "ServerLogic.hpp"
#include "IModel.hpp"
#include "ObjectMap.hpp"
#include "CommandTranslator.hpp"
#include "UpdateManager.hpp"
#include <ulib/thread/RecursiveMutex.hpp>

// forward references
class ISessionManager;
class IActionQueue;

/// world model
class SERVERLOGIC_DECLSPEC WorldModel: public IModel
{
public:
   /// ctor
   WorldModel(ISessionManager& sessionManager, IActionQueue& actionQueue)
      :m_actionQueue(actionQueue),
       m_commandTranslator(*this),
       m_updateManager(sessionManager)
   {
   }

   // virtual methods from IModel

   virtual void InitialUpdate(MobilePtr spPlayer) override;
   virtual void Tick(const TimeIndex& timeIndex) override;
   virtual void ReceiveAction(ActionPtr spAction) override;
   virtual void ReceiveCommand(Command& c) override;
   virtual void AddRemoveObject(const std::vector<ObjectPtr>& vecObjectsToAdd,
      const std::vector<ObjectId>& vecObjectsToRemove) override;
   virtual void UpdateObjectMovement(const ObjectId& id, const MovementInfo& info) override;

private:
   /// queues action
   void QueueAction(ActionPtr spAction);

private:
   /// mutex to protect object map
   RecursiveMutex m_mtxObjectMap;

   /// object map
   ObjectMap m_objectMap;

   /// action queue interface
   IActionQueue& m_actionQueue;

   /// command translator
   CommandTranslator m_commandTranslator;

   /// update manager
   UpdateManager m_updateManager;
};
