//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file LocalModel.hpp Local game model
//
#pragma once

// includes
#include "Network.hpp"
#include "IModel.hpp"
#include "ObjectMap.hpp"
#include <ulib/Observer.hpp>
#include "CommandTranslator.hpp"

/// model for local game
class NETWORK_DECLSPEC LocalModel: public IModel
{
   friend class LocalController;
   friend class ClientController;

public:
   /// ctor
   LocalModel();

   /// stops mobile movement (in case of disconnect)
   void Stop();

   // events used by view

   typedef Subject<void(ObjectPtr ptr)> T_SubjectOnAddObject;
   typedef Subject<void(const ObjectId& id)> T_SubjectOnRemoveObject;
   typedef Subject<void(const ObjectId& id, const MovementInfo& info)> T_SubjectOnUpdateObject;

   T_SubjectOnAddObject& SubjectOnAddObject() throw() { return m_subjectOnAddObject; }
   T_SubjectOnRemoveObject& SubjectOnRemoveObject() throw() { return m_subjectOnRemoveObject; }
   T_SubjectOnUpdateObject& SubjectOnUpdateObject() throw() { return m_subjectOnUpdateObject; }

   // access to model objects

   /// returns player object
   MobilePtr Player() throw() { return m_spPlayer; }
   /// returns player object; const version
   const MobilePtr Player() const throw() { return m_spPlayer; }

   /// returns object map
   ObjectMap& GetObjectMap() throw() { return m_objectMap; }
   /// returns object map; const version
   const ObjectMap& GetObjectMap() const throw() { return m_objectMap; }

protected:
   // virtual functions

   virtual void InitialUpdate(MobilePtr spPlayer) override;

   virtual void Tick(const TimeIndex& timeIndex) override;

   virtual void ReceiveAction(ActionPtr spAction) override;

   virtual void ReceiveCommand(Command& c) override;

   virtual void AddRemoveObject(const std::vector<ObjectPtr>& vecObjectsToAdd,
      const std::vector<ObjectId>& vecObjectsToRemove) override;

   virtual void UpdateObjectMovement(const ObjectId& id, const MovementInfo& info) override;

private:
   /// stops mobile movement
   void StopMobile(ObjectPtr spObject);

   /// lets mobiles "think"
   void ProcessMobiles();

   /// executes action
   void DoAction(ActionPtr spAction);

private:
   /// command translator
   CommandTranslator m_commandTranslator;

   /// object map
   ObjectMap m_objectMap;

   /// player mobile object
   MobilePtr m_spPlayer;

   /// subject for "AddObject" event
   T_SubjectOnAddObject m_subjectOnAddObject;
   /// subject for "RemoveObject" event
   T_SubjectOnRemoveObject m_subjectOnRemoveObject;
   /// subject for "UpdateObject" event
   T_SubjectOnUpdateObject m_subjectOnUpdateObject;
};
