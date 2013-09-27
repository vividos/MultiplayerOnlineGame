//
// Underworld - an Ultima Underworld remake project
// Copyright (C) 2002-2013 Michael Fink
//
//! \file Underworld/Renderer/LevelRenderer.hpp Underworld level renderer
//
#pragma once

// includes
#include "TexturedVertexBuffer.hpp"
#include "StaticIndexBuffer.hpp"

// forward references
class TextureAtlas;
class RenderWindow;
struct LevelWall;

namespace Underworld
{
class World;
class Tilemap;
class Level;
struct TileInfo;
}

/// \brief level renderer
class LevelRenderer
{
public:
   /// ctor
   LevelRenderer()
   {
   }

   /// prepares renderer for given level (e.g. when changing levels)
   void Prepare(TextureAtlas& atlas, const Underworld::Level& level);

   /// uploads vertices and indices
   void Upload();

   /// renders level
   void Render();

private:
   /// prepares tilemap
   void PrepareTilemap(TextureAtlas& atlas, const Underworld::Level& level);

   /// prepares single tile
   void PrepareTile(TextureAtlas& atlas, const Underworld::Tilemap& tilemap, unsigned int xpos, unsigned int ypos);

   /// adds level wall to vertex and index buffer
   void AddLevelWall(TextureAtlas& atlas, unsigned int xpos, unsigned int ypos, const Underworld::TileInfo& tileInfo, const LevelWall& wall);

   /// adds floor and ceiling for tile
   void AddLevelFloorCeiling(TextureAtlas& atlas, unsigned int xpos, unsigned int ypos, const Underworld::TileInfo& tileInfo);

private:
   /// vertex buffer
   TexturedVertexBuffer m_vertexBuffer;

   /// index buffer
   StaticIndexBuffer m_indexBuffer;
};
