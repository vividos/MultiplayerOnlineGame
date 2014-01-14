//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
//! \file AudioPlayerDlg.hpp Audio player dialog
//
#pragma once

// includes
#include "VirtualFileSystem.hpp"
#include "IAudioManager.hpp"
#include "MusicDirector.hpp"

/// about dialog
class AudioPlayerDlg : public CDialogImpl<AudioPlayerDlg>
{
public:
   AudioPlayerDlg();

   /// dialog id
   enum { IDD = IDD_AUDIOPLAYER };

private:
   // message map
   BEGIN_MSG_MAP(AudioPlayerDlg)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      COMMAND_ID_HANDLER(IDC_BUTTON_START_MUSICDIRECTOR, OnButtonStartMusicDirector)
      COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
      COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
   END_MSG_MAP()

private:
   /// called at start of dialog
   LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

   /// called to start music director
   LRESULT OnButtonStartMusicDirector(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

   /// called to close dialog
   LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
   /// file system
   VirtualFileSystem m_fileSystem;

   /// audio manager interface
   Audio::IAudioManager& m_audioManager;

   /// music director
   Audio::MusicDirector m_musicDirector;
};
