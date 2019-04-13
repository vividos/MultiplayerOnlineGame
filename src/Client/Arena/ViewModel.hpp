//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Arena/ViewModel.hpp Arena view model
//
#pragma once

// includes
#include <ulib/Observer.hpp>
#include "SelectionList.hpp"
#include "IPlayerViewModel.hpp"
#include "Mobile.hpp"

// forward references
class IModel;
class LocalModel;
class Player;
class ObjectMap;
class MovementInfo;

namespace Arena
{

/// the ViewModel (in the MVVM pattern) for the Arena project
class ViewModel: public IPlayerViewModel
{
public:
   /// ctor
   ViewModel(IModel& model);
   /// dtor
   ~ViewModel() {}

   // get methods

   /// returns local model
         LocalModel& GetLocalModel();
   /// returns local model
   const LocalModel& GetLocalModel() const;

   /// returns player object
   std::shared_ptr<Player> GetPlayer();

   /// returns player object
   std::shared_ptr<const Player> GetPlayer() const;

   /// returns object map
   const ObjectMap& GetObjectMap() const;

   /// returns player transparency
   double GetPlayerTransparency() const { return m_dPlayerTransparency; }


   // set methods


   // events

   /// subject type for add object event
   typedef Subject<void(MobilePtr)> T_AddObjectEvent;

   /// subject type for remove object event
   typedef Subject<void(const ObjectId&)> T_RemoveObjectEvent;

   /// subject type for update object event
   typedef Subject<void(const ObjectId&, const MovementInfo&)> T_UpdateObjectEvent;

   /// subject type for update player event
   typedef Subject<void(const Player&)> T_UpdatePlayerEvent;

   /// returns add object event
   T_AddObjectEvent& AddObjectEvent() { return m_addObjectEvent; }

   /// returns remove object event
   T_RemoveObjectEvent& RemoveObjectEvent() { return m_removeObjectEvent; }

   /// returns update object event
   T_UpdateObjectEvent& UpdateObjectEvent() { return m_updateObjectEvent; }

   /// returns update object event
   T_UpdatePlayerEvent& UpdatePlayerEvent() { return m_updatePlayerEvent; }

   // actions

   /// selects previous next mobile in selection list
   void SelectNextMobile(bool bDirectionNext = true);

   /// carries out whatever action is configured to the action button
   void DoAction(unsigned int uiActionNr);

private:
   // virtual methods from IPlayerViewModel

   // get methods

   /// returns player position
   virtual const Vector3d& GetPlayerPosition() const override;

   /// returns player movement angle
   virtual double GetPlayerMovementAngle() const override;

   /// returns if player view angle is controllable
   virtual bool IsPlayerViewAngleInControl() const override;

   // set methods

   /// sets player transparency
   virtual void SetPlayerTransparency(double dTransparency) override;

   // actions

   /// updates player movement
   virtual void UpdatePlayerMovement(const MovementInfo& movementInfo) override;

   /// updates player position by evaluating movement info
   virtual void UpdatePlayerPos() override;

   /// updates player view angle
   virtual void UpdatePlayerViewAngle(double dViewAngle) override;

private:
   /// list of selectable objects
   SelectionList m_selectionList;

   /// player transparency
   double m_dPlayerTransparency;

   /// action button configuration
   //ActionButtonConfig m_actionButtonConfig

   /// ref to Model (in the MVVM pattern)
   IModel& m_model;

   /// event for adding object
   T_AddObjectEvent m_addObjectEvent;

   /// event for removing object
   T_RemoveObjectEvent m_removeObjectEvent;

   /// event for updating object
   T_UpdateObjectEvent m_updateObjectEvent;

   /// event for updating player
   T_UpdatePlayerEvent m_updatePlayerEvent;
};

} // namespace Arena
