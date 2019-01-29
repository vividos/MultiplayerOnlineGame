//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file AudioPlayerDlg.cpp Audio player dialog
//

// includes
#include "stdafx.h"
#include "resource.h"
#include "AudioPlayerDlg.hpp"
#include <ulib/Path.hpp>

/// number of seconds for audio fadeout
const double c_dFadeoutTimeInSeconds = 2.0;

AudioPlayerDlg::AudioPlayerDlg()
:m_audioManager(Audio::IAudioManager::Get()),
 m_musicDirector(m_audioManager, m_fileSystem)
{
//   m_fileSystem.AddArchive(
//      Path::Combine(m_fileSystem.AppDataFolder(), _T("data.zip")));

   m_audioManager.SetFileStreamResolver(
      std::bind(&IFileSystem::OpenFile, &m_fileSystem, std::placeholders::_1, true));

   m_musicDirector.ReadMusicConfig();
}

LRESULT AudioPlayerDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   DoDataExchange(DDX_LOAD);

   m_positionalPosCtrl.Init(
      std::bind(&AudioPlayerDlg::UpdatePosition, this));

   CenterWindow(GetParent());

   SetupControls();

   UpdateVolumeControls();

   return TRUE;
}

LRESULT AudioPlayerDlg::OnHScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
   if (lParam == NULL)
      return 0;

   float fVolume = HIWORD(wParam) / 100.f;

   HWND hWnd = reinterpret_cast<HWND>(lParam);

   WORD wType = LOWORD(wParam);
   if (wType != SB_THUMBPOSITION && wType != SB_THUMBTRACK)
   {
      CTrackBarCtrl trackBar(hWnd);
      fVolume = trackBar.GetPos() / 100.f;
   }

   Audio::IVolumeControl& volumeControl = m_audioManager.GetVolumeControl();

   if (hWnd == m_tcMusicVolume.m_hWnd)
      volumeControl.SetVolume(Audio::IVolumeControl::enVolumeBackgroundMusic, fVolume);
   else
   if (hWnd == m_tcSoundVolume.m_hWnd)
      volumeControl.SetVolume(Audio::IVolumeControl::enVolumeUserInterface, fVolume);
   else
   if (hWnd == m_tcPositionalVolume.m_hWnd)
      volumeControl.SetVolume(Audio::IVolumeControl::enVolumePositional, fVolume);

   return 0;
}

LRESULT AudioPlayerDlg::OnButtonMusicPlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   CString cszMusicId;
   m_cbMusicId.GetWindowText(cszMusicId);

   m_audioManager.PlayMusic(cszMusicId);

   return 0;
}

LRESULT AudioPlayerDlg::OnButtonSoundPlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   CString cszSoundId;
   m_cbSoundId.GetWindowText(cszSoundId);

   m_spSoundPlaybackControl = m_audioManager.PlaySound(cszSoundId);
   return 0;
}

LRESULT AudioPlayerDlg::OnButtonSoundFadeout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   if (m_spSoundPlaybackControl != nullptr)
      m_spSoundPlaybackControl->Fadeout(c_dFadeoutTimeInSeconds);

   return 0;
}

LRESULT AudioPlayerDlg::OnButtonPositionalPlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   if (m_spPositionalSource == nullptr)
      m_spPositionalSource = m_audioManager.CreateSource();

   CString cszSoundId;
   m_cbPositionalId.GetWindowText(cszSoundId);

   m_spPositionalSource->SetPosition(m_positionalPosCtrl.GetPosition());

   m_spPositionalSource->Play(cszSoundId, true, true);

   return 0;
}

LRESULT AudioPlayerDlg::OnButtonPositionalFadeout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   if (m_spPositionalSource != nullptr)
      m_spPositionalSource->Fadeout(c_dFadeoutTimeInSeconds);

   return 0;
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

void AudioPlayerDlg::SetupControls()
{
   m_cbMusicId.AddString(_T("Arcadia"));
   m_cbMusicId.AddString(_T("Frost Waltz (Alternate)"));
   m_cbMusicId.AddString(_T("Schmetterling"));
   m_cbMusicId.SetCurSel(0);

   m_audioManager.AddSoundIdMapping(_T("Arcadia"), _T("audio\\music\\Arcadia.ogg"));
   m_audioManager.AddSoundIdMapping(_T("Frost Waltz (Alternate)"), _T("audio\\music\\Frost Waltz (Alternate).ogg"));
   m_audioManager.AddSoundIdMapping(_T("Schmetterling"), _T("audio\\music\\Schmetterling.ogg"));

   m_cbSoundId.AddString(_T("ring_inventory"));
   m_cbSoundId.SetCurSel(0);

   m_audioManager.AddSoundIdMapping(_T("ring_inventory"), _T("audio\\ui\\ring_inventory.ogg"));

   m_cbPositionalId.AddString(_T("amb_forest"));
   m_cbPositionalId.AddString(_T("amb_river"));
   m_cbPositionalId.SetCurSel(0);

   m_audioManager.AddSoundIdMapping(_T("amb_forest"), _T("audio\\sound\\amb_forest.ogg"));
   m_audioManager.AddSoundIdMapping(_T("amb_river"), _T("audio\\sound\\amb_river.ogg"));

   m_tcMusicVolume.SetRange(0, 100, FALSE);
   m_tcSoundVolume.SetRange(0, 100, FALSE);
   m_tcPositionalVolume.SetRange(0, 100, FALSE);
}

void AudioPlayerDlg::UpdatePosition()
{
   if (m_spPositionalSource != nullptr)
      m_spPositionalSource->SetPosition(m_positionalPosCtrl.GetPosition());
}

void AudioPlayerDlg::UpdateVolumeControls()
{
   Audio::IVolumeControl& volumeControl = m_audioManager.GetVolumeControl();

   float fVolume = volumeControl.GetVolume(Audio::IVolumeControl::enVolumeBackgroundMusic);
   m_tcMusicVolume.SetPos(static_cast<int>(fVolume * 100.0));

   fVolume = volumeControl.GetVolume(Audio::IVolumeControl::enVolumeUserInterface);
   m_tcSoundVolume.SetPos(static_cast<int>(fVolume * 100.0));

   fVolume = volumeControl.GetVolume(Audio::IVolumeControl::enVolumePositional);
   m_tcPositionalVolume.SetPos(static_cast<int>(fVolume * 100.0));
}
