//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2013 Michael Fink
//
//! \file Arena/GameClient.cpp Arena game client
//

// includes
#include "StdAfx.h"
#include "GameClient.hpp"
#include "MainGameScene.hpp"
#include "IWindowManager.hpp"
#include "IAudioManager.hpp"
#include <ulib/log/Appender.hpp>
#include <ulib/log/Layout.hpp>

using Arena::GameClient;

GameClient::GameClient()
:GameClientBase(_T("Arena")),
 m_uiAudioManager(Audio::IAudioManager::Get())
{
   SetupLogging();

   m_game.Init();
}

GameClient::~GameClient()
{
}

void GameClient::SetupLogging()
{
   Log::AppenderPtr spTraceAppender(new Log::OutputDebugStringAppender);

   Log::LayoutPtr spTraceLayout(new Log::PatternLayout(_T("%F(%L): log [%p] %m")));
   spTraceAppender->Layout(spTraceLayout);

   Log::Logger::GetRootLogger()->AddAppender(spTraceAppender);
}


void GameClient::Start()
{
   Init(800, 600, false);

   // enable audio events
   m_uiAudioManager.Connect(GetWindowManager());

   // start with main game scene
   ChangeScene(std::shared_ptr<Scene>(new MainGameScene(*this, *this, m_game)));

   Run();
}

void GameClient::OnTick()
{
   m_game.OnTick();
}
