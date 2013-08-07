//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file MobileActions.hpp Action classes related to Mobile
//
#pragma once

// includes
#include "Mobile.hpp"
#include "Action.hpp"

/// \brief action that a mobile selects another mobile
class SelectMobileAction: public Action
{
public:
   /// default ctor; used for receiving message
   SelectMobileAction()
      :Action(actionSelectMobile, ObjectId::Null(), ObjectRef(ObjectId::Null()))
   {
   }

   /// ctor; used for sending message
   SelectMobileAction(const ObjectId& mobileId, const ObjectId& selectedMobileId)
      :Action(actionSelectMobile, mobileId, ObjectRef(selectedMobileId))
   {
   }

   /// executes action
   virtual void Do(IModel& /*model*/) override
   {
      ObjectPtr sp = ArgumentRef().m_sp;

      // must be a mobile
      MobilePtr spMobile = boost::dynamic_pointer_cast<Mobile>(sp);

      const ObjectId& actorId = ActorId();
      spMobile->Selection(actorId);

//      LogAction(_T("SelectMobile"));
   }
};

/// \brief action that decreases health points of Mobile
/// Argument is the mobile to be decreased
class DecreaseHealthPointsAction: public Action
{
public:
   /// default ctor; used for receiving message
   DecreaseHealthPointsAction()
      :Action(actionDecreaseHealthPoints, ObjectId::Null(), ObjectRef(ObjectId::Null())),
       m_uiHealthPoints(0)
   {
   }

   /// ctor; used for sending message
   DecreaseHealthPointsAction(ObjectId actorId, ObjectId argumentId, unsigned int uiHealthPoints)
      :Action(actionDecreaseHealthPoints, actorId, ObjectRef(argumentId)),
       m_uiHealthPoints(uiHealthPoints)
   {
   }

   /// executes action
   virtual void Do(IModel& /*model*/) override
   {
      ObjectPtr sp = ArgumentRef().m_sp;

      // must be a mobile
      MobilePtr spMobile = boost::dynamic_pointer_cast<Mobile>(sp);
      unsigned int uiCurrentHealthPoints = spMobile->HealthPoints();
      if (m_uiHealthPoints < uiCurrentHealthPoints)
         uiCurrentHealthPoints -= m_uiHealthPoints;
      else
         uiCurrentHealthPoints = 0;

      spMobile->HealthPoints(uiCurrentHealthPoints);

//      LogAction(_T("DecreaseHealthPoints"));
   }

   /// serialize action by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override
   {
      Action::Serialize(stream);
      stream.Write32(m_uiHealthPoints);
   }

   /// deserialize action by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override
   {
      Action::Deserialize(stream);
      m_uiHealthPoints = stream.Read32();
   }

private:
   /// health points to decrease by
   unsigned int m_uiHealthPoints;
};
