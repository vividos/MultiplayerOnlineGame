//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file ViewModel.cpp Arena view model
//

// includes
#include "StdAfx.h"
#include "ViewModel.hpp"
#include "IModel.hpp"
#include "LocalModel.hpp"
#include "Player.hpp"
#include "Command.hpp"

using Arena::ViewModel;

ViewModel::ViewModel(IModel& model)
:m_model(model),
 m_dPlayerTransparency(0.0)
{
}

std::shared_ptr<Player> ViewModel::GetPlayer()
{
   LocalModel& model = dynamic_cast<LocalModel&>(m_model);

   return std::dynamic_pointer_cast<Player>(model.Player());
}

const ObjectMap& ViewModel::GetObjectMap() const
{
   const LocalModel& model = dynamic_cast<const LocalModel&>(m_model);
   return model.GetObjectMap();
}

void ViewModel::SelectNextMobile(bool bDirectionNext)
{
   m_selectionList.Update(GetPlayer()->Pos(), GetObjectMap(), c_dMaxSelectionDistance);

   bDirectionNext ? m_selectionList.SelectNext() : m_selectionList.SelectPrev();

   Command c(cmdSelectObject, m_selectionList.GetSelected(), GetPlayer()->Id());
   m_model.ReceiveCommand(c);
}

void ViewModel::DoAction(unsigned int /*uiActionNr*/)
{
   //ATLASSERT(uiActionNr < 10);
   // TODO use m_actionButtonConfig to choose action

   // alternative impl here:
   MobilePtr spPlayer = GetPlayer();

   if (spPlayer->Selection() == ObjectId::Null())
      return; // nothing selected

   Command c(cmdHitMobile, spPlayer->Id(), spPlayer->Selection());
   m_model.ReceiveCommand(c);
}

const Vector3d& ViewModel::GetPlayerPosition() const
{
   return const_cast<ViewModel&>(*this).GetPlayer()->Pos();
}

double ViewModel::GetPlayerMovementAngle() const
{
   return const_cast<ViewModel&>(*this).GetPlayer()->ViewAngle();
}

bool ViewModel::IsPlayerViewAngleInControl() const
{
   // always; may change when model takes over controlling player
   return true;
}

void ViewModel::SetPlayerTransparency(double dTransparency)
{
   m_dPlayerTransparency = dTransparency;
}

void ViewModel::UpdatePlayerMovement(const MovementInfo& movementInfo)
{
   // TODO
   GetPlayer()->UpdateMovementInfo(movementInfo);
   // send to model
   //m_controller.MovePlayer(movementInfo);
}

void ViewModel::UpdatePlayerPos()
{
   GetPlayer()->Move();
}

void ViewModel::UpdatePlayerViewAngle(double dViewAngle)
{
   // TODO send to controller, when at least 1s has elapsed yet

   // apply to player
   GetPlayer()->ViewAngle(static_cast<unsigned int>(dViewAngle));
}
