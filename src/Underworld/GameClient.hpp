//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/GameClient.hpp Game client
//
#pragma once

// includes
#include "stdafx.h"
#include "GameClientBase.hpp"
#include "IngameScene.hpp"
#include "Game.hpp"

/// game client
class GameClient : public GameClientBase
{
public:
   /// ctor
   GameClient()
      :GameClientBase(_T("Underworld"))
   {
   }
   /// dtor
   virtual ~GameClient()
   {
   }

   /// starts game client
   void Start()
   {
      GameClientBase::Init(800, 600, false);

      m_game.Init();

      ISceneManager& sceneManager = *this;

      std::shared_ptr<Scene> spScene =
         std::make_shared<IngameScene>(sceneManager, GetRenderEngine(), GetKeyboardActionManager(), m_game);
      sceneManager.ChangeScene(spScene);

      GameClientBase::Run();

      m_game.Done();
   }

private:
   /// game objects
   Game m_game;
};
