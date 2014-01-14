//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file CommandTranslator.cpp Command to Action translator
//

// includes
#include "StdAfx.h"
#include "CommandTranslator.hpp"
#include "Command.hpp"
#include "IModel.hpp"
#include "MobileActions.hpp"

CommandTranslator::CommandTranslator(IModel& model)
:m_model(model)
{
}

CommandTranslator::~CommandTranslator() throw()
{
}

void CommandTranslator::ReceiveCommand(Command& cmd)
{
   switch (cmd.Id())
   {
   case cmdNull:
      break;

   case cmdSelectObject:
      {
         std::shared_ptr<SelectMobileAction> spAction(
            new SelectMobileAction(cmd.ActorId(), cmd.ArgumentId()));

         m_model.ReceiveAction(spAction);
      }
      break;

   case cmdHitMobile:
      {
         // translate command to action
         std::shared_ptr<DecreaseHealthPointsAction> spDecAction(
            new DecreaseHealthPointsAction(cmd.ActorId(), cmd.ArgumentId(), 1));

         ActionPtr spAction(spDecAction);

         m_model.ReceiveAction(spAction);
      }
      break;

   default:
      {
         CString cszText;
         cszText.Format(_T("unknown command id=%04x"), cmd.Id());
         throw Exception(cszText, __FILE__, __LINE__);
      }
   }
}

bool CommandTranslator::CheckCommand(const Command& /*cmd*/)
{
   // TODO implement
   return true;
}
