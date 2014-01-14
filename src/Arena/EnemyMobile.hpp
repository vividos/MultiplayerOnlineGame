//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file EnemyMobile.hpp Enemy mobile
//
#pragma once

// includes
#include "MobileActor.hpp"
#include "Vector3d.hpp"

// forward references
class LocalModel;

namespace Arena
{

/// enemy mobile actor
class EnemyMobile: public MobileActor
{
public:
   /// ctor
   EnemyMobile(LocalModel& model, const Vector3d& vOffset = Vector3d());
   /// dtor
   virtual ~EnemyMobile() throw() {}

   // actions

   /// lets mobile think about what to do next
   virtual void Think(IModel& /*model*/, IActionQueue& /*actionQueue*/) override;

   /// updates movement of enemy
   void UpdateMovement();

private:
   /// local model
   LocalModel& m_model;

   /// think timer
   Timer m_timer;

   /// think stage
   unsigned int m_uiStage;

   /// points to run through
   std::vector<Vector3d> m_vecPoints;
};

} // namespace Arena
