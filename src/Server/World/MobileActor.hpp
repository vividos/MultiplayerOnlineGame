//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file MobileActor.hpp Mobile actor
//
#pragma once

// includes
#include "World.hpp"
#include "Mobile.hpp"

// forward references
class IModel;
class IActionQueue;
class ThreatList;

/// \brief actor behind a Mobile object
/// \details used on server side to evaluate AI of mobile
class WORLD_DECLSPEC MobileActor : public Mobile
{
public:
   /// movement strategy
   enum T_enMovementStrategy
   {
      movementIdle,     ///< mobile wanders around idle
      movementRunTo,    ///< mobile runs towards point; use planned path
      movementStand,    ///< mobile stands to attack, cast, etc.
      movementFlee,     ///< mobile flees
   };

   /// ctor
   MobileActor(const ObjectId& id);
   /// dtor
   virtual ~MobileActor() {}

   // get methods
   const ThreatList& GetThreatList() const { ATLASSERT(m_spThreatList != NULL); return *m_spThreatList; }
         ThreatList& GetThreatList() { ATLASSERT(m_spThreatList != NULL); return *m_spThreatList; }

   // actions

   /// lets mobile think about what to do next
   virtual void Think(IModel& model, IActionQueue& actionQueue);

private:
   /// actor's threat list
   std::shared_ptr<ThreatList> m_spThreatList;

   /// currently planned path to follow
   //std::shared_ptr<Path> m_spCurrentPath;
};

/// mobile actor shared ptr
typedef std::shared_ptr<MobileActor> MobileActorPtr;
