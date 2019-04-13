//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file CharGenerator/Controller.cpp Character generator controller
//

// includes
#include "stdafx.h"
#include "Controller.hpp"
#include "PerspectiveCamera.hpp"
#include "Vector3.hpp"

using CharGenerator::Controller;

Controller::Controller(KeyboardActionManager& /*keyboardActionManager*/,
   std::shared_ptr<PerspectiveCamera> spCamera)
:m_spCamera(spCamera)
{
   spCamera->SetPosition(Vector3d(0.0, 2.0, -2.5), 180.0, -20.0);
}

Controller::~Controller()
{
}
