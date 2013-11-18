//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Arena\Game.cpp Game objects
//

// includes
#include "stdafx.h"
#include "Game.hpp"
#include "EnemyMobile.hpp"
#include "Player.hpp"

using Arena::Game;

Game::Game()
{
}

void Game::Init()
{
   m_tickTimer.Start();

   MobilePtr spPlayer(new Player(Uuid::New()));
   spPlayer->Pos(Vector3d(8.0, 0.0, 8.0));

   m_model.InitialUpdate(spPlayer);

   // add opponent
   //MobilePtr spEnemy(new Arena::EnemyMobile(m_model));
   //m_model.GetObjectMap().AddObject(spEnemy);

   //// add opponent
   //MobilePtr spEnemy2(new Arena::EnemyMobile(m_model, Vector3d(20.0, 0.0, 30.0)));
   //m_model.GetObjectMap().AddObject(spEnemy2);
}

void Game::OnTick()
{
   if (m_tickTimer.Elapsed() < 0.2) // 5 times per second
      return;

   // do model tick
   m_model.Tick(m_timeBase.Now());

   m_tickTimer.Restart();
}
