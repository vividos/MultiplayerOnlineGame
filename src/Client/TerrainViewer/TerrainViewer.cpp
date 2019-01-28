//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TerrainViewer.cpp Terrain viewer app
//

// includes
#include "stdafx.h"
#include "TerrainViewer.hpp"
#include "TerrainViewerMainScene.hpp"
#include "CrashReporter.hpp"
#include "Filesystem.hpp"

void TerrainViewer::Client::Start()
{
   Init(800, 600, false);

   ChangeScene(std::shared_ptr<Scene>(
      new TerrainViewer::MainScene(
         *this,
         GetGraphicsTaskManager(),
         GetRenderEngine(),
         GetKeyboardActionManager())));

   Run();
}

/// TerrainViewer main function
int APIENTRY _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
                       LPTSTR    /*lpCmdLine*/, int       /*nCmdShow*/)
{
   CrashReporter::Init(Filesystem().BaseFolder() + c_pszCrashdumpFoldername);

   try
   {
      TerrainViewer::Client gc;
      gc.Start();
   }
   catch(Exception& ex)
   {
      CString cszText;
      cszText.Format(_T("TerrainViewer ended with exception: %s"), ex.Message().GetString());
      MessageBox(GetActiveWindow(), cszText, _T("TerrainViewer"), MB_OK);
   }

   return 0;
}
