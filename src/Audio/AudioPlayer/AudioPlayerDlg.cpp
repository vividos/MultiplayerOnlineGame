//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file AudioPlayerDlg.cpp Audio player dialog
//

// includes
#include "stdafx.h"
#include "resource.h"
#include "AudioPlayerDlg.hpp"
#include "Path.hpp"

AudioPlayerDlg::AudioPlayerDlg()
:m_audioManager(Audio::IAudioManager::Get()),
 m_musicDirector(m_audioManager, m_fileSystem)
{
//   m_fileSystem.AddArchive(
//      Path::Combine(m_fileSystem.AppDataFolder(), _T("data.zip")));

   m_musicDirector.ReadMusicConfig();
}

LRESULT AudioPlayerDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   CenterWindow(GetParent());

   return TRUE;
}   

LRESULT AudioPlayerDlg::OnButtonStartMusicDirector(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   m_musicDirector.Start();
   return 0;
}

LRESULT AudioPlayerDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   EndDialog(wID);
   return 0;
}
