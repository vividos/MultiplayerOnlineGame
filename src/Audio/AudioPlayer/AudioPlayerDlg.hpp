//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AudioPlayerDlg.hpp Audio player dialog
//
#pragma once

// includes
#include "VirtualFileSystem.hpp"
#include "IAudioManager.hpp"
#include "MusicDirector.hpp"
#include "PositionalPosCtrl.hpp"

/// audio player dialog
class AudioPlayerDlg :
   public CDialogImpl<AudioPlayerDlg>,
   public CWinDataExchange<AudioPlayerDlg>
{
public:
   AudioPlayerDlg();

   /// dialog id
   enum { IDD = IDD_AUDIOPLAYER };

private:
   // data exchange map
   BEGIN_DDX_MAP(AudioPlayerDlg)
      DDX_CONTROL_HANDLE(IDC_COMBO_MUSIC_ID, m_cbMusicId)
      DDX_CONTROL_HANDLE(IDC_COMBO_SOUND_ID, m_cbSoundId)
      DDX_CONTROL_HANDLE(IDC_COMBO_POSITIONAL_ID, m_cbPositionalId)
      DDX_CONTROL_HANDLE(IDC_SLIDER_MUSIC_VOLUME, m_tcMusicVolume)
      DDX_CONTROL_HANDLE(IDC_SLIDER_SOUND_VOLUME, m_tcSoundVolume)
      DDX_CONTROL_HANDLE(IDC_SLIDER_POSITIONAL_VOLUME, m_tcPositionalVolume)
      DDX_CONTROL(IDC_STATIC_POSITIONAL_POS, m_positionalPosCtrl)
   END_DDX_MAP()

   // message map
   BEGIN_MSG_MAP(AudioPlayerDlg)
      MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
      COMMAND_ID_HANDLER(IDC_BUTTON_MUSIC_PLAY, OnButtonMusicPlay)
      //COMMAND_ID_HANDLER(IDC_BUTTON_MUSIC_FADEOUT, OnButtonMusicFadeout)
      MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
      COMMAND_ID_HANDLER(IDC_BUTTON_SOUND_PLAY, OnButtonSoundPlay)
      COMMAND_ID_HANDLER(IDC_BUTTON_SOUND_FADEOUT, OnButtonSoundFadeout)
      COMMAND_ID_HANDLER(IDC_BUTTON_POSITIONAL_PLAY, OnButtonPositionalPlay)
      COMMAND_ID_HANDLER(IDC_BUTTON_POSITIONAL_FADEOUT, OnButtonPositionalFadeout)
      COMMAND_ID_HANDLER(IDC_BUTTON_START_MUSICDIRECTOR, OnButtonStartMusicDirector)
      COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
      COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
      REFLECT_NOTIFICATIONS()
   END_MSG_MAP()

private:
   /// called at start of dialog
   LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

   /// called when track bar controls are moved
   LRESULT OnHScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

   /// called when button Play for music is pressed
   LRESULT OnButtonMusicPlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

   /// called when button Play for sound is pressed
   LRESULT OnButtonSoundPlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

   /// called when button Fadeout for sound is pressed
   LRESULT OnButtonSoundFadeout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

   /// called when button Play for positional sound is pressed
   LRESULT OnButtonPositionalPlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

   /// called when button Fadeout for positional sound is pressed
   LRESULT OnButtonPositionalFadeout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

   /// called to start music director
   LRESULT OnButtonStartMusicDirector(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

   /// called to close dialog
   LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

   /// sets up all controls
   void SetupControls();
   /// updates positional sound position
   void UpdatePosition();

   /// updates volume based on track bar controls
   void UpdateVolumeControls();

private:
   // UI

   CComboBox m_cbMusicId;        ///< music combobox
   CComboBox m_cbSoundId;        ///< sound combobox
   CComboBox m_cbPositionalId;   ///< positional sound combobox

   CTrackBarCtrl m_tcMusicVolume;      ///< music volume track bar
   CTrackBarCtrl m_tcSoundVolume;      ///< sound volume track bar
   CTrackBarCtrl m_tcPositionalVolume; ///< positional sound volume track bar

   /// positional position control
   PositionalPosCtrl m_positionalPosCtrl;

   // Model

   /// file system
   VirtualFileSystem m_fileSystem;

   /// audio manager interface
   Audio::IAudioManager& m_audioManager;

   /// music director
   Audio::MusicDirector m_musicDirector;

   /// sound playback control
   std::shared_ptr<Audio::IPlaybackControl> m_spSoundPlaybackControl;

   /// positional audio source
   std::shared_ptr<Audio::IPositionalSource> m_spPositionalSource;
};
