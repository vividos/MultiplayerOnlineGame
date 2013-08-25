//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Mobile.hpp Mobile class
//
#pragma once

// includes
#include "Object.hpp"
#include "MobileDisplayInfo.hpp"
#include "MovementInfo.hpp"
#include "Timer.hpp"
#include <set>

/// \brief mobile
/// A mobile is an object that can interact with the world, including the
/// player. A mobile may be controlled by AI or by another player.
class COMMON_DECLSPEC Mobile : public Object
{
public:
   /// ctor
   Mobile(const Uuid& id)
      :Object(id),
       m_displayInfo(0),
       m_uiMovementAngle(0),
       m_uiViewAngle(0),
       m_uiHealthPoints(100),
       m_selection(Uuid::Null())
   {
      m_timerMovement.Start();
   }

   /// dtor
   virtual ~Mobile() throw() {}

   // get methods

   /// display info
   const MobileDisplayInfo& DisplayInfo() const throw() { return m_displayInfo; }

   /// movement info
   const MovementInfo& GetMovementInfo() const throw() { return m_movementInfo; }

   /// movement walk angle
   unsigned int MovementAngle() const throw() { return m_uiMovementAngle; }

   /// view angle of mobile
   unsigned int ViewAngle() const throw() { return m_uiViewAngle; }

   /// number of health points
   unsigned int HealthPoints() const throw() { return m_uiHealthPoints; }

   /// returns selected mobile
   const Uuid& Selection() const throw() { return m_selection; }

   // set methods

   /// sets display info
   void DisplayInfo(const MobileDisplayInfo& displayInfo) throw() { m_displayInfo = displayInfo; }

   /// sets view angle
   void ViewAngle(unsigned int uiViewAngle) throw() { m_uiViewAngle = uiViewAngle; }

   /// sets movement angle
   void MovementAngle(unsigned int uiMovementAngle) throw() { m_uiMovementAngle = uiMovementAngle; }

   /// sets new number of health points
   void HealthPoints(unsigned int uiHealthPoints) throw() { m_uiHealthPoints = uiHealthPoints; }

   /// sets new selection
   void Selection(const Uuid& selection) throw() { m_selection = selection; }

   // actions

   /// updates object
   void UpdateMovementInfo(const MovementInfo& info);

   /// moves mobile, according to movement info
   virtual void Move();

   // serialize

   /// serialize message by putting bytes to stream
   virtual void Serialize(ByteStream& stream) const override;

   /// deserialize message by reading bytes from stream
   virtual void Deserialize(ByteStream& stream) override;

private:
   /// display info
   MobileDisplayInfo m_displayInfo;

   /// movement info
   MovementInfo m_movementInfo;

   /// timer for movement
   Timer m_timerMovement;

   /// movement angle
   unsigned int m_uiMovementAngle;

   /// view angle
   unsigned int m_uiViewAngle;

   /// current selection
   Uuid m_selection;

   /// health points
   unsigned int m_uiHealthPoints;
};

/// Mobile smart pointer
typedef std::shared_ptr<Mobile> MobilePtr;
