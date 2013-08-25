//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Controller.hpp Controller base class
//
#pragma once

// includes
#include "IController.hpp"

/// \brief controller base class
class Controller: public IController
{
public:
   // methods

   /// sets next controller to send commands to (or NULL)
   void SetNextController(std::shared_ptr<IController> spController)
   {
      m_spNextController = spController;
   }

protected:
   /// next controller
   std::shared_ptr<IController> m_spNextController;
};
