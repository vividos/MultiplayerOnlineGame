//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file Arena/TerrainRenderManager.hpp Arena terrain render manager
//

// includes
#include "StdAfx.h"
#include "TerrainRenderManager.hpp"
#include "DataSource/MandelbrotTerrainDataSource.hpp"
#include "DataSource/SineCosineTerrainDataSource.hpp"

Arena::TerrainRenderManager::TerrainRenderManager(RenderEngine& engine,
   GraphicsTaskManager& taskManager)
:Terrain::View::TerrainRenderManager(engine, taskManager, m_dataBlockManager),
m_spDataSource(new Terrain::MandelbrotTerrainDataSource),
//m_spDataSource(new Terrain::SineCosineTerrainDataSource),
m_dataBlockManager(taskManager, m_spDataSource)
{
}
