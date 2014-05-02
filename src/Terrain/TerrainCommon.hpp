//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file TerrainCommon.hpp Terrain project include
//
#pragma once

// includes
#include "Base.hpp"

/// export macro
#ifdef TERRAIN_EXPORTS
#  define TERRAIN_DECLSPEC __declspec(dllexport)
#else
#  define TERRAIN_DECLSPEC __declspec(dllimport)
#endif

/// \brief Terrain rendering classes
/// \details The terrain rendering classes are connected this way:
/// \dot
/// digraph terrainclasses {
///    node [shape=box]
///    Scenegraph [ label="Scenegraph" URL="\ref Terrain::Scenegraph"];
///    DataBlockManager [ label="DataBlockManager" URL="\ref Terrain::Model::DataBlockManager"];
///    IDataSource [ label="IDataSource" URL="\ref Terrain::IDataSource"];
///    DataBlock [ label="DataBlock" URL="\ref Terrain::Model::DataBlock"];
///    IBlockRenderData [ label="IBlockRenderData" URL="\ref Terrain::IBlockRenderData"];
///    LevelOfDetail::BlockRenderData [ label="LevelOfDetail::BlockRenderData" URL="\ref Terrain::LevelOfDetail::BlockRenderData"];
///    DataBlockManager [ label="DataBlockManager" URL="\ref Terrain::Model::DataBlockManager"];
///    IActiveVertexMap [ label="IActiveVertexMap" URL="\ref Terrain::IActiveVertexMap"];
///    ReduceAlgorithm [ label="ReduceAlgorithm" URL="\ref Terrain::ReduceAlgorithmBase"];
///    ITrianglesCompiler [ label="ITrianglesCompiler" URL="\ref Terrain::ITrianglesCompiler"];
///    ReducedTrianglesCompilerVertexBuffer [ label="ReducedTrianglesCompilerVertexBuffer" URL="\ref Terrain::ReducedTrianglesCompilerVertexBuffer"];
///
///    Scenegraph -> DataBlockManager [ label="has" ]
///    Scenegraph -> IDataSource [ label="has" ]
///    DataBlockManager -> DataBlock [ label="has many" ]
///    DataBlock -> IBlockRenderData [ label="has" ]
///    IBlockRenderData -> BlockRenderDataReducedTriangles [ label="is implemented by" ]
///    BlockRenderDataReducedTriangles -> DataBlockManager [ label="uses worker thread from" ]
///    BlockRenderDataReducedTriangles -> IActiveVertexMap [ label="has" ]
///    BlockRenderDataReducedTriangles -> ReduceAlgorithm [ label="uses" ]
///    BlockRenderDataReducedTriangles -> ITrianglesCompiler [ label="uses" ]
///    ITrianglesCompiler -> ReducedTrianglesCompilerVertexBuffer [ label="is implemented by" ]
/// }
/// \enddot
namespace Terrain
{
}
