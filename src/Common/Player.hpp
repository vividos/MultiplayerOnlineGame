//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Common/Player.hpp Player class
//
#pragma once

// includes
#include "Mobile.hpp"
//#include "Inventory.hpp"

/// player mobile
class Player: public Mobile
{
public:
   /// ctor
   Player(const ObjectId& id)
      :Mobile(id)
   {
   }

   /// dtor
   virtual ~Player() throw() {}

private:
   /// inventory
   //Inventory m_inventory;
};
