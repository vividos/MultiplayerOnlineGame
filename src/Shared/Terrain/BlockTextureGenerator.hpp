//
// MultiplayerOnlineGame - multiplayer game project
// Copyright (C) 2008-2014 Michael Fink
//
/// \file BlockTextureGenerator.hpp Texture generator for terrain block
//
#pragma once

// includes
#include "Texture.hpp"
#include "NamedTextureMap.hpp"

namespace Terrain
{
   // forward references
   namespace Model
   {
      class DataBlock;
   }

   /// texture generator for blocks
   class BlockTextureGenerator
   {
   public:
      /// ctor
      BlockTextureGenerator();
      /// dtor
      ~BlockTextureGenerator();

      /// prepares all used textures
      void Prepare();

      /// uploads all used textures
      void Upload();

      /// generates texture for given data block; must be called in render thread
      TexturePtr Generate(std::shared_ptr<Model::DataBlock> spDataBlock);

   private:
      /// renders zone textures
      void RenderZoneTextures();

      /// renders paths to texture
      void RenderPaths();

      /// renders decals for block
      void RenderDecals();

   private:
      /// default texture
      TexturePtr m_spDefaultTexture;

      /// all textures for block
      NamedTextureMap m_textureMap;
   };

} // namespace Terrain
