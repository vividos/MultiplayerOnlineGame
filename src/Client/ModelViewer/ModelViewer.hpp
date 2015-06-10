//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file ModelViewer.hpp Model viewer app
//

// includes
#include "stdafx.h"
#include "GameClientBase.hpp"
#include "VirtualFileSystem.hpp"

namespace ModelViewer
{
   /// model viewer client
   class Client : public GameClientBase
   {
   public:
      /// ctor
      Client()
         :GameClientBase(_T("Model Viewer"))
      {
      }
      /// dtor
      virtual ~Client() throw() {}

      /// starts viewer
      void Start();

   private:
      /// file system
      VirtualFileSystem m_fileSystem;
   };

} // namespace ModelViewer
