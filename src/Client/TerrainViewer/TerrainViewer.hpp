//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TerrainViewer.hpp Terrain viewer classes
//
#pragma once

// includes
#include "GameClientBase.hpp"

namespace TerrainViewer
{
   /// terrain viewer client
   class Client : public GameClientBase
   {
   public:
      /// ctor
      Client() throw()
         :GameClientBase(_T("Terrain Viewer"))
      {
      }

      /// starts app
      void Start();
   };

} // namespace TerrainViewer
