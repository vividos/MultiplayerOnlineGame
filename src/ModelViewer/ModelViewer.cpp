//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ModelViewer.cpp Model viewer app
//

// includes
#include "stdafx.h"
#include "ModelViewer.hpp"
#include "ModelViewerScene.hpp"
#include "CrashReporter.hpp"
#include "Filesystem.hpp"
#include "DefaultActionKeyDefs.hpp"

void ModelViewer::Client::Start()
{
   bool bFullscreen = false;
   Init(800, 600, bFullscreen);

   DefaultActionKeyDefs::Register(GetKeyboardActionManager());

   ChangeScene(std::shared_ptr<Scene>(
      new ModelViewer::Scene(*this,
         GetWindowManager(),
         GetKeyboardActionManager(),
         m_fileSystem,
         GetRenderEngine(),
         GetGraphicsTaskManager())));

   Run();
}

/// ModelViewer main function
int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
   CrashReporter::Init(Filesystem().BaseFolder() + c_pszCrashdumpFoldername);

   try
   {
      ModelViewer::Client client;
      client.Start();
   }
   catch (Exception& ex)
   {
      CString cszText;
      cszText.Format(_T("Model Viewer ended with exception: %s"), ex.Message());
      MessageBox(GetActiveWindow(), cszText, _T("ModelViewer"), MB_OK);
   }

   return 0;
}
