//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Arena/ViewModel.cpp Arena view model
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

LocalModel& ViewModel::GetLocalModel() throw()
{
   return dynamic_cast<LocalModel&>(m_model);
}

const LocalModel& ViewModel::GetLocalModel() const throw()
{
   return dynamic_cast<const LocalModel&>(m_model);
}

std::shared_ptr<Player> ViewModel::GetPlayer()
{
   return std::dynamic_pointer_cast<Player>(GetLocalModel().Player());
}

std::shared_ptr<const Player> ViewModel::GetPlayer() const
{
   return std::dynamic_pointer_cast<const Player>(GetLocalModel().Player());
}

const ObjectMap& ViewModel::GetObjectMap() const
{
   return GetLocalModel().GetObjectMap();
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
   return GetPlayer()->Pos();
}

double ViewModel::GetPlayerMovementAngle() const
{
   return GetPlayer()->ViewAngle();
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
   GetPlayer()->UpdateMovementInfo(movementInfo);

   // update view
   UpdatePlayerEvent().Call(*GetPlayer().get());
}

void ViewModel::UpdatePlayerPos()
{
   GetPlayer()->Move();

   // update view
   UpdatePlayerEvent().Call(*GetPlayer().get());
}

void ViewModel::UpdatePlayerViewAngle(double dViewAngle)
{
   // apply to player
   GetPlayer()->ViewAngle(static_cast<unsigned int>(dViewAngle));

   // update view
   UpdatePlayerEvent().Call(*GetPlayer().get());
}
