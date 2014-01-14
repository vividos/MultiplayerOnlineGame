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
#include "LoadingScene.hpp"
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

#if 1
   std::shared_ptr<MainGameScene> spMainGameScene(new MainGameScene(*this, *this, m_game));
   ChangeScene(spMainGameScene);
#else
   // prepare loading screen
   std::shared_ptr<LoadingScene> spLoadingScene(new LoadingScene(*this, *this, m_game.GetFileSystem()));

   spMainGameScene->Prepare(spLoadingScene->GetPreloadManager());

   spLoadingScene->FinishQueue([&](){
      // start main game scene
      ChangeScene(spMainGameScene);
   });

   ChangeScene(spLoadingScene);
#endif

   Run();
}

void GameClient::OnTick()
{
   m_game.OnTick();
}
