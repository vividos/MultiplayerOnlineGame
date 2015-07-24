//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AttachedConsole.cpp Attached console class
//

// include
#include "stdafx.h"
#include "AttachedConsole.hpp"
#include <io.h> // for _open_osfhandle
#include <fcntl.h> // for _O_TEXT

using namespace Win32;

AttachedConsole::AttachedConsole()
:m_bCalledFromConsole(false)
{
   BOOL bRet = ::AttachConsole(ATTACH_PARENT_PROCESS);
   if (bRet == FALSE)
   {
      DWORD dwError = GetLastError();
      if (dwError == ERROR_ACCESS_DENIED)
      {
         // this happens when used in a console based app; do nothing, we already have a console
         m_bCalledFromConsole = true;
         return;
      }
      else
      {
         m_bCalledFromConsole = false;
         return;
      }
   }

   m_bCalledFromConsole = true;

   ReadPrompt(m_cszPrompt);

   SetupRedirect();
}

AttachedConsole::~AttachedConsole() throw()
{
   Detach();
}

void AttachedConsole::Detach() throw()
{
   if (m_bCalledFromConsole && !m_cszPrompt.IsEmpty())
   {
      _tprintf(_T("%s"), m_cszPrompt.GetString());
      m_cszPrompt.Empty();

      ATLVERIFY(TRUE == FreeConsole());
   }
}

void AttachedConsole::ReadPrompt(CString& cszPrompt)
{
   // read console prompt
   HANDLE hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE); // don't close the handle
   ATLASSERT(hConsole != INVALID_HANDLE_VALUE);

   // get cursor pos
   CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
   ATLVERIFY(TRUE == GetConsoleScreenBufferInfo(hConsole, &consoleScreenBufferInfo));

   COORD cursorPos;
   cursorPos.X = consoleScreenBufferInfo.srWindow.Left;
   cursorPos.Y = consoleScreenBufferInfo.dwCursorPosition.Y;

   const unsigned int uiReadChars = consoleScreenBufferInfo.dwCursorPosition.X;

   // read prompt
   DWORD dwNumberOfCharsRead = 0;

   ::ReadConsoleOutputCharacter(hConsole,
      cszPrompt.GetBuffer(uiReadChars),
      uiReadChars,
      cursorPos, &dwNumberOfCharsRead);

   cszPrompt.ReleaseBuffer(dwNumberOfCharsRead);

   // set cursor to first column
   ::SetConsoleCursorPosition(hConsole, cursorPos);

   // overwrite prompt
   CString cszBlank(_T(' '), uiReadChars);
   ATLVERIFY(TRUE == ::WriteConsole(hConsole, cszBlank, uiReadChars, NULL, NULL));

   // set cursor again
   ::SetConsoleCursorPosition(hConsole, cursorPos);
}

void AttachedConsole::SetupRedirect()
{
   // redirect unbuffered STDOUT to the console
   long lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
   int hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

   FILE* fp = _fdopen(hConHandle, "w");
   *stdout = *fp;

   setvbuf(stdout, NULL, _IONBF, 0);

   // stderr, stdin not redirected yet

   // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
   // point to console as well
   std::ios::sync_with_stdio();
}
