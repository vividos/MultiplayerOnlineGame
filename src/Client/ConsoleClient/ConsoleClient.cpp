//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ConsoleClient.cpp Console based client
//

// includes
#include "stdafx.h"
#include "ConsoleClient.hpp"
#include "ConsoleClientSession.hpp"
#include <ulib/log/Appender.hpp>
#include <ulib/log/Layout.hpp>
#include "CommonDefines.hpp"
#include "AuthInfo.hpp"
#include "PingMessages.hpp"
#include "TextMessage.hpp"
#include <ulib/Event.hpp>
#include <iostream>

ConsoleClient::ConsoleClient()
:m_ioService(true, _T("Console Background Thread"))
{
   SetupLogging();
}

void ConsoleClient::Run()
{
   m_ioService.Run();

   Sleep(100);

   for (;;)
   {
      StartConnect();

      // wait for connect
      bool bConnect = true;
      WaitForConnect(bConnect);

      if (!bConnect)
         continue;

      Sleep(100);

      RunCommandLoop();

      m_spSession->Disconnect();
      m_spSession.reset();

      _tprintf(_T("disconnected.\n"));

      Sleep(2*100);
   }
}

void ConsoleClient::SetupLogging()
{
   Log::AppenderPtr spConsoleAppender(new Log::ConsoleAppender);
   Log::AppenderPtr spTraceAppender(new Log::OutputDebugStringAppender);

   Log::LayoutPtr spLayout(new Log::SimpleLayout);
   spConsoleAppender->Layout(spLayout);
   spTraceAppender->Layout(spLayout);

   Log::Logger::GetRootLogger()->AddAppender(spConsoleAppender);
   Log::Logger::GetRootLogger()->AddAppender(spTraceAppender);
}

void ConsoleClient::StartConnect()
{
   _tprintf(_T("\nLogin\n"));
   _tprintf(_T("Username: "));

   // read in line
   std::string strUsername;
   std::getline(std::cin, strUsername);

   _tprintf(_T("Password: "));

   std::string strPassword;
   std::getline(std::cin, strPassword);

   _tprintf(_T("\n"));

   AuthInfo authInfo(strUsername.c_str(), strPassword.c_str());

   m_spSession.reset(new ConsoleClientSession(m_ioService.Get()));

   m_spSession->SetAuthInfo(authInfo);
   m_spSession->Connect(c_pszDefaultServerHostname, c_usDefaultServerPort);
}

void ConsoleClient::WaitForConnect(bool& bConnect)
{
   bConnect = true;
   while (!m_spSession->GetConnectSuccessEvent().Wait(1000))
   {
      if (m_spSession->ConnectState() == ClientSession::connectStateAuthFailed ||
          m_spSession->ConnectState() == ClientSession::connectStateConnectError)
      {
         bConnect = false;
         break;
      }
   }
}

void ConsoleClient::RunCommandLoop()
{
   bool bExit = false;
   do
   {
      _tprintf(_T("Commands:\n"));
      _tprintf(_T("p                ping\n"));
      _tprintf(_T("s {text}         say\n"));
      _tprintf(_T("w {name} {text}  whisper\n"));
      _tprintf(_T("x                exit\n"));

      _tprintf(_T(":>"));

      std::string strText;
      std::getline(std::cin, strText);

      if (strText.length() == 0)
         continue;

      if (strText[0] == 'p')
      {
         _tprintf(_T("Ping message\n"));
         PingRequestMessage msg;
         m_spSession->SendMessage(msg);
      }
      else
      if (strText[0] == 's')
      {
         CString cszText(strText.c_str() + 1);
         cszText.Trim();

         TextMessage msg(TextMessage::textMsgSay, cszText);
         m_spSession->SendMessage(msg);

         _tprintf(_T("You: %s\n"), cszText.GetString());
      }
      else
      if (strText[0] == 'w')
      {
         TextMessage msg;
         _tprintf(_T("sending whisper message\n"));
      }
      else
      if (strText[0] == 'x')
         bExit = true;

   } while(!bExit);
}

/// console client main function
int _tmain(int argc, _TCHAR* argv[])
{
   _tprintf(_T("MultiplayerOnlineGame Console Client\n\n"));
   argc; argv;

   ConsoleClient c;
   c.Run();

   return 0;
}
