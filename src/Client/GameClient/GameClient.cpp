//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file src/GameClient/GameClient.cpp Main function
//

// includes
#include "stdafx.h"
#include <ulib/CrashReporter.hpp>
#include "GameClient.hpp"
#include "Filesystem.hpp"
#include "IAudioManager.hpp"
#include <ulib/log/ConsoleAppender.hpp>
#include <ulib/log/OutputDebugStringAppender.hpp>
#include <ulib/log/PatternLayout.hpp>
#include "LogFileAppender.hpp"

GameClient::GameClient()
:GameClientBase(_T("Game Client")),
m_uiAudioManager(Audio::IAudioManager::Get())
{
   SetupLogging();
}

void GameClient::SetupLogging()
{
   {
      Log::LayoutPtr spTraceLayout(new Log::PatternLayout(_T("%F(%L): log [%p] %m")));

      Log::AppenderPtr spTraceAppender(new Log::OutputDebugStringAppender);
      spTraceAppender->Layout(spTraceLayout);

      Log::Logger::GetRootLogger()->AddAppender(spTraceAppender);
   }

   {
      Log::LayoutPtr spLogFileLayout(new Log::PatternLayout(_T("%d [%p] [%c] %m")));

      Log::AppenderPtr spLogFileAppender(new LogFileAppender);
      spLogFileAppender->Layout(spLogFileLayout);

      Log::Logger::GetRootLogger()->AddAppender(spLogFileAppender);
   }
}

void GameClient::Start()
{
   Init(800, 600, false);

   // enable audio events
   m_uiAudioManager.Connect(GetWindowManager(), m_fileSystem);

   //std::shared_ptr<MainGameScene> spMainGameScene(new MainGameScene(*this, *this, m_game));
   //ChangeScene(spMainGameScene);

   Run();
}

/// GameClient main function
int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
   CrashReporter::Init(_T("gameclient"), Filesystem().UserFolder() + c_pszCrashdumpFoldername);

   try
   {
      GameClient gc;
      gc.Start();
   }
   catch (Exception& ex)
   {
      CString cszText;
      cszText.Format(_T("Arena ended with exception: %s"), ex.Message().GetString());
      MessageBox(GetActiveWindow(), cszText, _T("Arena"), MB_OK);
   }

   return 0;
}
