//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TestAudioManager.cpp Unit tests for class AudioManager
//

// includes
#include "stdafx.h"
#include "IAudioManager.hpp"
#include "Vector3d.hpp"

namespace UnitTest
{

/// tests class AudioManager
TEST_CLASS(TestAudioManager)
{
   /// tests IAudioManager
   TEST_METHOD(Test1)
   {
      Audio::IAudioManager& mgr = Audio::IAudioManager::Get();

      std::shared_ptr<Audio::ISource> spSource = mgr.CreateSource();

      Vector3d vPos(-1.0, 0.0, 0.0);
      spSource->SetPosition(vPos);

      spSource->Play(_T("egal"), false, false);

      //std::shared_ptr<Audio::IPlaybackControl> spPlaybackControl = mgr.PlaySound(true, _T("egal"));
      //Sleep(30*1000);

      spSource->Fadeout(0.0);
   }
};

} // namespace UnitTest
