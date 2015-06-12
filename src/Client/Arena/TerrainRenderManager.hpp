//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file src/Arena/TerrainRenderManager.cpp Arena terrain render manager
//
#pragma once

// includes
#include "View/TerrainRenderManager.hpp"
#include "Model/DataBlockManager.hpp"

// forward references
namespace Terrain
{
   class IDataSource;
}

namespace Arena
{
   /// terrain render manager for Arena
   class TerrainRenderManager : public Terrain::View::TerrainRenderManager
   {
   public:
      /// ctor
      TerrainRenderManager(RenderEngine& engine, GraphicsTaskManager& taskManager);
      /// dtor
      virtual ~TerrainRenderManager() throw() {}

   private:
      /// data source
      std::shared_ptr<Terrain::IDataSource> m_spDataSource;

      /// data block manager
      Terrain::Model::DataBlockManager m_dataBlockManager;
   };

} // namespace Arena
