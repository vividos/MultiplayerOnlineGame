//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ActionRegistry.cpp Action registry
//

// includes
#include "StdAfx.h"
#include "ActionRegistry.hpp"
#include "Action.hpp"
#include "MobileActions.hpp"

ActionPtr ActionRegistry::CreateById(unsigned short usActionId)
{
   switch(usActionId)
   {
   case actionDecreaseHealthPoints:
      return ActionPtr(new DecreaseHealthPointsAction);
      break;
   };

   CString cszText;
   cszText.Format(_T("Unknown action id=%04x"), usActionId);
   throw Exception(cszText, __FILE__, __LINE__);
}
